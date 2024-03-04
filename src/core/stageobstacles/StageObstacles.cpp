/**
 * @file StageObstacles.cpp
 * @author Tomáš Ludrovan
 * @brief StageObstacles class
 * @version 0.1
 * @date 2024-01-14
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "core/stageobstacles/StageObstacles.hpp"

#ifndef OLD_TRAJECTORY_ALGORITHM

void StageObstacles::initializeCollisionObjects(
	const std::vector<StageObstacle>& obstacles, const Size2d& bounds)
{
	for (const auto& obstacle: obstacles) {
		addObstacleToCollisionObjects(obstacle);
	}
	addBoundsToCollisionObjects(bounds);
}

void StageObstacles::addObstacleToCollisionObjects(
	const StageObstacle& obstacle)
{
	Triangle_2 trg(toCgalTriangle(obstacle));
	if (trg.orientation() == CGAL::Orientation::CLOCKWISE) {
		// The obstacle edges must be CCW
		trg = trg.opposite();
	}
	m_collObjs.push_back(std::move(trg));
}

void StageObstacles::addBoundsToCollisionObjects(const Size2d& bounds)
{
	constexpr int EDGE_COUNT = 4;
	Point_2 corners[EDGE_COUNT] = {
		Point_2(0, 0),                // top left
		Point_2(bounds.w, 0),         // top right
		Point_2(bounds.w, bounds.h),  // bottom right
		Point_2(0, bounds.h),         // bottom left
	};
	// These can be any points that complete the triangle and are on the
	// negative side of the respective edge.
	Point_2 edgeVertexes3[EDGE_COUNT] = {
		Point_2(-10, 0),            // left
		Point_2(0, -10),            // top
		Point_2(bounds.w + 10, 0),  // right
		Point_2(0, bounds.h + 10),  // bottom
	};

	Point_2 p, q, r;

	q = corners[EDGE_COUNT - 1];
	for (int i = 0; i < EDGE_COUNT; i++) {
		p = std::move(q);
		q = corners[i];
		r = edgeVertexes3[i];
		Triangle_2 trg(p, q, r);
		m_collObjs.push_back(std::move(trg));
	}
}

StageObstacles::StageObstacles(
	const std::vector<StageObstacle>& obstacles, const Size2d& bounds)
	: m_obstacles{obstacles}
	, m_bounds{bounds}
{
	initializeCollisionObjects(obstacles, bounds);
}

const std::vector<StageObstacle>& StageObstacles::getObstaclesList() const
{
	return m_obstacles;
}

const Size2d& StageObstacles::getStageSize() const
{
	return m_bounds;
}

bool hasCollision(const std::vector<Triangle_2>& collObjs,
	const Segment_2& seg, double playerRadius)
{
	for (const auto& collObj : collObjs) {
		if (CGAL::squared_distance(collObj, seg) < CGAL::square(playerRadius)) {
			return true;
		}
	}
	return false;
}

Trajectory StageObstacles::getPlayerTrajectory(const Point_2& playerPos,
	const Vector_2& playerMove, double playerRadius)
{
	// Square of the maximum error of the bisection method
	static constexpr double SQR_MAX_ERROR = sqr(0.5);

	// Source (starting point)
	Point_2 sp(playerPos);
	// Target (end point)
	Point_2 ep(playerPos + playerMove);

	if (hasCollision(m_collObjs, Segment_2(sp, ep), playerRadius)) {
		Vector_2 bumpVect = playerMove;
		
		// Aproximate collision point using bisection method
		while (bumpVect.squared_length() > SQR_MAX_ERROR) {
			// Bisect
			bumpVect /= 2;

			if (hasCollision(m_collObjs, Segment_2(sp, ep), playerRadius)) {
				// Inside collision object -- go back

				ep -= bumpVect;
			} else {
				// Outside collision object -- go forward

				ep += bumpVect;
			}
		}

		if (hasCollision(m_collObjs, Segment_2(sp, ep), playerRadius)) {
			// Aproximation ended up inside the collision object -- undo the
			// last "bump"

			ep -= bumpVect;

			// Positive side is in the direction of the player movement
			Line_2 l(playerPos, playerMove.perpendicular(
				CGAL::COUNTERCLOCKWISE));
			if (!l.has_on_positive_side(ep)) {
				// Player is moving in opposite direction -- fix that!

				ep = playerPos;
				// Now the player is not moving at all
			}
		}
	}

	Trajectory res(Segment_2(sp, ep), playerMove);
	return res;
}

#else // OLD_TRAJECTORY_ALGORITHM

#include "functions.hpp"

class Obstacle {
public:
	enum Type {
		OBST_WALL,
		OBST_CORNER,
	};
	typedef Segment_2 Wall;
	typedef Circle_2 Corner;
private:
	const Type m_type;
	Wall m_wall;
	Corner m_corner;
public:
	/**
	 * @brief Constructs a new Obstacle object of type `WALL`.
	 */
	Obstacle(const Wall& w)
		: m_type{OBST_WALL}
		, m_wall{w}
	{}
	/**
	 * @brief Constructs a new Obstacle object of type `CORNER`.
	 */
	Obstacle(const Corner& c)
		: m_type{OBST_CORNER}
		, m_corner{c}
	{}

	/**
	 * @brief Returns the obstacle type.
	 */
	Type getType() const { return m_type; }

	/**
	 * @brief Gives access to wall obstacle.
	 * 
	 * @remark Obstacle type must be `WALL`.
	 */
	const Wall& wall() const {
		assert(m_type == OBST_WALL);
		return m_wall;
	}

	/**
	 * @brief Gives access to corner obstacle.
	 * 
	 * @remark Obstacle type must be `CORNER`.
	 */
	const Corner& corner() const {
		assert(m_type == OBST_CORNER);
		return m_corner;
	}

	bool hasOn(const Point_2& pt) const {
		switch (m_type) {
			case OBST_WALL: return m_wall.has_on(pt);
			case OBST_CORNER: return m_corner.has_on_boundary(pt);
		}

		// Default
		return false;
	}

	/**
	 * @brief Returns the direction of the normal vector of the obstacle at
	 *        the `pt`.
	 */
	Direction_2 getNormal(const Point_2& pt) const {
		assert(hasOn(pt));
		switch (m_type) {
			case OBST_WALL: {
				auto v = m_wall.to_vector();
				auto n = v.perpendicular(CGAL::CLOCKWISE);
				return n.direction();
			} break;
			case OBST_CORNER: {
				Vector_2 v(m_corner.center(), pt);
				return v.direction();
			} break;
		}

		// Default
		return Direction_2();
	}
};

class Collision {
public:
	enum Type {	
		COLL_WALL,
		COLL_CORNER,
	};
	struct Wall {
		Segment_2 seg;

		/**
		 * @brief Returns the vector parallel to the wall line segment.
		 */
		Vector_2 vector() const { return seg.to_vector(); }
		/**
		 * @brief Returns the source point of the wall line segment.
		 */
		Point_2 pStart() const { return seg.source(); }
		/**
		 * @brief Returns the target point of the wall line segment.
		 */
		Point_2 pEnd() const { return seg.target(); }
	};
	struct Corner {
		Circle_2 circ;

		/**
		 * @brief Returns the center of the corner circle.
		 */
		Point_2 center() const { return circ.center(); }
	};
private:
	const Type m_type;
	Wall m_wall;
	Corner m_corner;

	Point_2 m_collPoint;

	/**
	 * @brief Get the collision tangent knowing that the collision type
	 *        is `WALL`.
	 */
	Line_2 getTangentWall() const {
		auto res = m_wall.seg.supporting_line();
		return res;
	}
	/**
	 * @brief Get the collision tangent knowing that the collision type
	 *        is `CORNER`.
	 */
	Line_2 getTangentCorner() const {
		Vector_2 v1(m_corner.center(), m_collPoint);
		Vector_2 v2 = v1.perpendicular(CGAL::COUNTERCLOCKWISE);
		Line_2 res(m_collPoint, v2);
		return res;
	}

	static Type collisionTypeFromObstacle(const Obstacle& obst) {
		switch (obst.getType()) {
			case Obstacle::OBST_WALL:
				return COLL_WALL;
			case Obstacle::OBST_CORNER:
				return COLL_CORNER;
		}

		// Default
		return Type(-1);
	}
	static Wall wallFromObstacle(const Obstacle& obst) {
		switch (obst.getType()) {
			case Obstacle::OBST_WALL:
				return Wall{.seg = obst.wall()};
			case Obstacle::OBST_CORNER: break;
		}
		
		return Wall();
	}
	static Corner cornerFromObstacle(const Obstacle& obst) {
		switch (obst.getType()) {
			case Obstacle::OBST_WALL: break;
			case Obstacle::OBST_CORNER:
				return Corner{.circ = obst.corner()};
		}
		
		return Corner();
	}
public:
	/**
	 * @brief Constructs a new Collision object of type `WALL`.
	 * 
	 * @param w Wall.
	 * @param collPt Collision point.
	 */
	Collision(const Wall& w, const Point_2& collPt)
		: m_type{COLL_WALL}
		, m_wall{w}
		, m_collPoint{collPt}
	{
		//assert(w.seg.has_on(collPt));
	}
	/**
	 * @brief Constructs a new Collision object of type `CORNER`.
	 * 
	 * @param c Corner.
	 * @param collPt Collision point.
	 */
	Collision(const Corner& c, const Point_2& collPt)
		: m_type{COLL_CORNER}
		, m_corner{c}
		, m_collPoint{collPt}
	{
		//assert(c.circ.has_on_boundary(collPt));
	}
	/**
	 * @brief Constructs a new Collision object.
	 * 
	 * @param obst Obstacle.
	 * @param collPt Collision point.
	 */
	Collision(const Obstacle& obst, const Point_2& collPt)
		: m_type{collisionTypeFromObstacle(obst)}
		, m_wall{wallFromObstacle(obst)}
		, m_corner{cornerFromObstacle(obst)}
		, m_collPoint{collPt}
	{}

	/**
	 * @brief Returns the type of the collision.
	 */
	Type getType() const { return m_type; }

	/**
	 * @brief Gives access to the wall collision.
	 * 
	 * @remark Collision type must be `WALL`.
	 */
	Wall& wall() {
		assert(m_type == COLL_WALL);
		return m_wall;
	}
	/**
	 * @brief Gives access to the wall collision.
	 * 
	 * @remark Collision type must be `WALL`.
	 */
	const Wall& wall() const {
		assert(m_type == COLL_WALL);
		return m_wall;
	}

	/**
	 * @brief Gives access to the corner collision.
	 * 
	 * @remark Collision type must be `CORNER`.
	 */
	Corner& corner() {
		assert(m_type == COLL_CORNER);
		return m_corner;
	}
	/**
	 * @brief Gives access to the corner collision.
	 * 
	 * @remark Collision type must be `CORNER`.
	 */
	const Corner& corner() const {
		assert(m_type == COLL_CORNER);
		return m_corner;
	}

	/**
	 * @brief Returns the point of collision.
	 */
	const Point_2& collPoint() const { return m_collPoint; }

	/**
	 * @brief Returns the tangent line of the collision.
	 */
	Line_2 tangent() const {
		switch (m_type) {
			case COLL_WALL: return getTangentWall();
			case COLL_CORNER: return getTangentCorner();
		}

		// Default
		return Line_2();
	}
};

struct TrajectoryConstructionData {
	Point_2 playerPosition;
	Vector_2 playerMovementVector;
	std::vector<Obstacle> obstacles;
};

static constexpr CGAL::Orientation FLEX_CLOCKWISE = CGAL::CLOCKWISE;
static constexpr CGAL::Orientation FLEX_COUNTERCLOCKWISE
	= CGAL::COUNTERCLOCKWISE;
static constexpr CGAL::Orientation FLEX_BOTH = CGAL::Orientation(0);

static
boost::optional<boost::variant<std::vector<Point_2>,Segment_2,Quadrant_arc_2>>
intersection(const TrajectorySegment& trajSeg, const Obstacle& obst);
static Trajectory constructTrajectory(TrajectoryConstructionData& data);
static std::vector<Collision> getCollisionsWith(
	TrajectoryConstructionData& data, const TrajectorySegment& trajSeg,
	const Obstacle& obst);
static boost::optional<Collision> findCollision(
	TrajectoryConstructionData& data, const TrajectorySegment& trajSeg);
static TrajectorySegment initTrajSeg(TrajectoryConstructionData& data,
	const Collision& coll);
static CGAL::Orientation getFlexOrientation(TrajectoryConstructionData& data,
	const Collision& coll);
static Point_2 findCollisionZoneEnd(TrajectoryConstructionData& data,
	const Collision& coll);
static Collision shortestPath(TrajectoryConstructionData& data,
	const TrajectorySegment& trajSeg, const std::vector<Collision>& collisions);


void StageObstacles::initializeCollisionObjects(
	const std::vector<StageObstacle>& obstacles, const Size2d& bounds)
{
	initializeWalls(obstacles, bounds);
	initializeCorners(obstacles, bounds);
}

void StageObstacles::initializeWalls(
	const std::vector<StageObstacle>& obstacles, const Size2d& bounds)
{
	for (const auto& obstacle : obstacles) {
		addObstacleToWallsList(obstacle);
	}
	addBoundsToWallsList(bounds);
}

void StageObstacles::initializeCorners(
	const std::vector<StageObstacle>& obstacles, const Size2d& bounds)
{
	for (const auto& obstacle : obstacles) {
		addObstacleToCornersList(obstacle);
	}
	addBoundsToCornersList(bounds);
}

Vector_2 StageObstacles::getWallUnitNormal(const Segment_2& wallSeg)
{
	// Vector of the wall
	Vector_2 wallSegVect = wallSeg.to_vector();
	// Vector perpendicular to the wall
	Vector_2 wallSegVectPerp = wallSegVect.perpendicular(
		CGAL::Orientation::CLOCKWISE);
	// Length of the perpendicular vector
	auto wallSegVectPerpLen = CGAL::sqrt(wallSegVectPerp.squared_length());
	// Result
	Vector_2 wallSegVectPerpUnit = wallSegVectPerp / wallSegVectPerpLen;

	return wallSegVectPerpUnit;
}

void StageObstacles::addObstacleToWallsList(const StageObstacle& obstacle)
{
	constexpr int EDGE_COUNT = 3;

	Triangle_2 trg(toCgalTriangle(obstacle));
	if (trg.orientation() == CGAL::Orientation::CLOCKWISE) {
		// The obstacle edges must be CCW
		trg = trg.opposite();
	}

	Point_2 sp, ep;
	Wall wall;

	ep = trg.vertex(EDGE_COUNT - 1);
	for (int i = 0; i < EDGE_COUNT; i++) {
		sp = std::move(ep);
		ep = trg.vertex(i);
		addWallToList(sp, ep);
	}
}

void StageObstacles::addBoundsToWallsList(const Size2d& bounds)
{
	constexpr int EDGE_COUNT = 4;
	Point_2 corners[EDGE_COUNT] = {
		Point_2(0, 0),
		Point_2(bounds.w, 0),
		Point_2(bounds.w, bounds.h),
		Point_2(0, bounds.h),
	};

	Point_2 sp, ep;

	ep = corners[EDGE_COUNT - 1];
	for (int i = 0; i < EDGE_COUNT; i++) {
		sp = std::move(ep);
		ep = corners[i];
		addWallToList(sp, ep);
	}
}

void StageObstacles::addObstacleToCornersList(const StageObstacle& obstacle)
{
	for (int i = 0; i < obstacle.CORNER_COUNT; i++) {
		addCornerToList(toCgalPoint(obstacle.corners[i]));
	}
}

void StageObstacles::addBoundsToCornersList(const Size2d& bounds)
{
	(void)bounds;
	// The player will never collide with a stage corner, only the walls
}

void StageObstacles::addWallToList(const Point_2& sp, const Point_2& ep)
{
	addWallToList(Segment_2(sp, ep));
}

void StageObstacles::addWallToList(const Segment_2& seg)
{
	Wall wall{.seg = seg, .n = getWallUnitNormal(seg)};
	m_walls.push_back(std::move(wall));
}

void StageObstacles::addCornerToList(const Point_2& p)
{
	Corner corner{.p = p};
	m_corners.push_back(std::move(corner));
}

StageObstacles::StageObstacles(
	const std::vector<StageObstacle>& obstacles, const Size2d& bounds)
	: m_obstacles{obstacles}
	, m_bounds{bounds}
{
	initializeCollisionObjects(obstacles, bounds);
}

const std::vector<StageObstacle>& StageObstacles::getObstaclesList() const
{
	return m_obstacles;
}

const Size2d& StageObstacles::getStageSize() const
{
	return m_bounds;
}

Trajectory StageObstacles::getPlayerTrajectory(const Point_2& playerPos,
	const Vector_2& playerMove, double playerRadius)
{
	TrajectoryConstructionData data;

	data.playerPosition = playerPos;
	data.playerMovementVector = playerMove;

	for (const auto& wall : m_walls) {
		Vector_2 offsetVect = wall.n * playerRadius;
		Aff_transformation_2 transf(CGAL::TRANSLATION, offsetVect);
		Segment_2 offsetSeg = wall.seg.transform(transf);
		data.obstacles.push_back(Obstacle(offsetSeg));
	}

	for (const auto& corner : m_corners) {
		Circle_2 cornerCirc(corner.p, CGAL::square(playerRadius));
		data.obstacles.push_back(Obstacle(cornerCirc));
	}

	auto res = constructTrajectory(data);
	return res;
}

boost::optional<boost::variant<std::vector<Point_2>,Segment_2,Quadrant_arc_2>>
intersection(const TrajectorySegment& trajSeg, const Obstacle& obst)
{
	typedef boost::optional<boost::variant<std::vector<Point_2>,Segment_2,
		Quadrant_arc_2>> ReturnValue;

	if (trajSeg.getType() == TrajectorySegment::TRAJECTORY_LINE_SEGMENT
		&& obst.getType() == Obstacle::OBST_WALL)
	{
		// Line segment -> wall

		auto isec = intersection(trajSeg.lineSegment().seg, obst.wall());
		if (!isec) {
			return boost::none;
		}
		else if (auto isecPt = boost::get<Point_2>(&*isec)) {
			std::vector res({*isecPt});
			return ReturnValue(res);
		}
		else {
			auto isecSeg = boost::get<Segment_2>(&*isec);
			assert(isecSeg);
			return ReturnValue(*isecSeg);
		}
	}
	else if (trajSeg.getType() == TrajectorySegment::TRAJECTORY_LINE_SEGMENT
		&& obst.getType() == Obstacle::OBST_CORNER)
	{
		// Line segment -> corner

		auto isec = bounds_intersection(trajSeg.lineSegment().seg,
			obst.corner());
		if (!isec) {
			return boost::none;
		}
		else {
			auto isecPts = boost::get<std::vector<Point_2>>(&*isec);
			assert(isecPts);
			return ReturnValue(*isecPts);
		}
	}
	else if (trajSeg.getType() == TrajectorySegment::TRAJECTORY_ARC
		&& obst.getType() == Obstacle::OBST_WALL)
	{
		// Arc -> wall

		auto isec = intersection(trajSeg.arc().arc, obst.wall());
		if (!isec) {
			return boost::none;
		}
		else {
			auto isecPts = boost::get<std::vector<Point_2>>(&*isec);
			assert(isecPts);
			return ReturnValue(*isecPts);
		}
	}
	else {
		assert(trajSeg.getType() == TrajectorySegment::TRAJECTORY_ARC
			&& obst.getType() == Obstacle::OBST_CORNER);
		// Arc -> corner

		auto isec = bounds_intersection(trajSeg.arc().arc, obst.corner());
		if (!isec) {
			return boost::none;
		}
		else {
			auto isecPts = boost::get<std::vector<Point_2>>(&*isec);
			assert(isecPts);
			return ReturnValue(*isecPts);
		}
	}
}

Trajectory constructTrajectory(TrajectoryConstructionData& data)
{
	Trajectory res;
	Vector_2 vTmp;

	boost::optional<TrajectorySegment> tail = boost::none;

	auto moveEndPt = data.playerPosition + data.playerMovementVector;
	vTmp = data.playerMovementVector.perpendicular(CGAL::CLOCKWISE);
	// The direction of the line may be any arbitrary direction as long as it is
	// not parallel (=> collinear) with the player movement. The more important
	// thing about it is that it must cross through the MoveEndPt point.
	Line_2 moveEndLine(moveEndPt, vTmp);

	CGAL::Orientation flexible = FLEX_BOTH;

	Segment_2 segment0seg(data.playerPosition, moveEndPt);
	auto segment0 = TrajectorySegment::constructLineSegment(std::move(
		segment0seg));
	res.push(std::move(segment0));

	if (segment0.getPStart() == segment0.getPEnd()) {
		// Doesn't move

		return res;
	}

	while (true) {
		auto coll = findCollision(data, res.last());
		if (coll == boost::none) {
			// No collision found

			if (tail == boost::none) {
				// Done

				return res;
			} else {
				res.push(*tail);
				flexible = FLEX_BOTH;
				tail = boost::none;
			}
		} else {
			// Process collision

			// The collision happened before the tail -> clear it
			tail = boost::none;
			// Modify the last trajectory segment
			res.last().setPEnd(coll->collPoint());
			// Create new trajectory segment
			auto newSegment = initTrajSeg(data, *coll);

			auto refl = coll->tangent();
			if (CGAL::angle(refl.to_vector(), data.playerMovementVector)
				== CGAL::RIGHT)
			{
				// Bumped straight into the obstacle -> stop moving here

				newSegment.setPEnd(coll->collPoint());
				res.push(std::move(newSegment));
				return res;
			}

			auto newFlexible = getFlexOrientation(data, *coll);
			if (flexible != FLEX_BOTH && newFlexible != flexible) {
				// In the corner with nowhere to move further -> stop

				newSegment.setPEnd(coll->collPoint());
				res.push(std::move(newSegment));
				return res;
			}
			flexible = newFlexible;

			Line_2 l1(coll->collPoint(), data.playerMovementVector);
			auto isec1 = CGAL::intersection(l1, moveEndLine);
			moveEndPt = *boost::get<Point_2>(&*isec1);
			vTmp = refl.to_vector().perpendicular(CGAL::CLOCKWISE);
			moveEndLine = Line_2(moveEndPt, vTmp);

			auto collisionZoneEnd = findCollisionZoneEnd(data, *coll);
			vTmp = refl.to_vector().perpendicular(CGAL::CLOCKWISE);
			Line_2 l2(collisionZoneEnd, vTmp);

			auto isec2 = CGAL::intersection(moveEndLine, refl);
			auto p1 = *boost::get<Point_2>(&*isec2);
			auto isec3 = CGAL::intersection(l2, refl);
			auto p2 = *boost::get<Point_2>(&*isec3);

			if (CGAL::squared_distance(coll->collPoint(), p1)
				<= CGAL::squared_distance(coll->collPoint(), p2))
			{
				switch (coll->getType()) {
					case Collision::COLL_WALL: {
						newSegment.setPEnd(p1);
					} break;
					case Collision::COLL_CORNER: {
						auto isec4 = bounds_intersection(coll->corner().circ,
							moveEndLine);
						auto isecPts = boost::get<std::vector<Point_2>>(
							&*isec4);
						if (isecPts->size() == 1) {
							newSegment.setPEnd(isecPts->at(0));
						} else {
							if (CGAL::squared_distance(p1, isecPts->at(0)) <
								CGAL::squared_distance(p1, isecPts->at(1)))
							{
								newSegment.setPEnd(isecPts->at(0));
							} else {
								newSegment.setPEnd(isecPts->at(1));
							}
						}
					} break;
				}
				res.push(std::move(newSegment));
			}
			else {
				newSegment.setPEnd(collisionZoneEnd);
				res.push(std::move(newSegment));

				Line_2 l3(collisionZoneEnd, data.playerMovementVector);
				auto isec4 = CGAL::intersection(l3, moveEndLine);
				// TODO: is there always an intersection?
				auto tailSegEp = *boost::get<Point_2>(&*isec4);
				Segment_2 tailSeg(collisionZoneEnd, tailSegEp);
				tail = TrajectorySegment::constructLineSegment(tailSeg);
			}
		}
	}
}

std::vector<Collision> getCollisionsWith(TrajectoryConstructionData& data,
	const TrajectorySegment& trajSeg, const Obstacle& obst)
{
	(void)data;

	std::vector<Collision> res;

	auto isec = intersection(trajSeg, obst);
	if (!isec) {
		// No common point
		return res;
	}
	else if (static_cast<bool>(boost::get<Segment_2>(&*isec))
		|| static_cast<bool>(boost::get<Quadrant_arc_2>(&*isec)))
	{
		// Merely touching the obstacle
		return res;
	}
	else {
		auto intersections = boost::get<std::vector<Point_2>>(&*isec);
		assert(intersections);

		for (const auto& isecPt : *intersections) {
			if (isecPt == trajSeg.getPEnd()) {
				// Merely touching the obstacle
				continue;
			}
			if (isecPt == trajSeg.getPStart()) {
				auto mvDir = trajSeg.directionAt(isecPt).to_vector();
				auto obstNormal = obst.getNormal(isecPt).to_vector();
				if (CGAL::angle(mvDir, obstNormal) == CGAL::ACUTE) {
					// Moving away from the obstacle, assuming that the player is not able to
					// cross the obstacle from inside.
					continue;
				}
			}
			Collision coll(obst, isecPt);
			res.push_back(std::move(coll));
		}
	}

	return res;
}

boost::optional<Collision> findCollision(TrajectoryConstructionData& data,
	const TrajectorySegment& trajSeg)
{
	std::vector<Collision> collisions;
	for (const auto& obst : data.obstacles) {
		auto colls = getCollisionsWith(data, trajSeg, obst);
		vectorAppendRange(collisions, colls);
	}

	if (collisions.empty()) {
		return boost::none;
	} else {
		return shortestPath(data, trajSeg, collisions);
	}
}

TrajectorySegment initTrajSeg(TrajectoryConstructionData& data,
	const Collision& coll)
{
	(void)data;

	switch (coll.getType()) {
		case Collision::COLL_WALL: {
			// The `target` may have any value, because it is not supposed to
			// be initialized here
			Segment_2 seg(coll.collPoint(), coll.collPoint());
			return TrajectorySegment::constructLineSegment(seg);
		} break;
		case Collision::COLL_CORNER: {
			// The `target` may have any value, because it is not supposed to
			// be initialized here
			Quadrant_arc_2 arc(coll.corner().center(), coll.collPoint(),
				coll.collPoint());
			return TrajectorySegment::constructArc(arc);
		} break;
	}
	
	// Default
	assert(((void)"Invalid collision type", false));
}

CGAL::Orientation getFlexOrientation(TrajectoryConstructionData& data,
	const Collision& coll)
{
	switch (coll.getType()) {
		case Collision::COLL_WALL: {
			if (CGAL::angle(coll.wall().vector(), data.playerMovementVector)
				== CGAL::ACUTE)
			{
				return FLEX_CLOCKWISE;
			} else {
				return FLEX_COUNTERCLOCKWISE;
			}
		} break;
		case Collision::COLL_CORNER: {
			// Note the minus sign -- it has opposite direction!
			Line_2 moveLine(coll.collPoint(), -data.playerMovementVector);
			return moveLine.oriented_side(coll.corner().center());
		} break;
	}
	
	// Default
	assert(((void)"Invalid collision type", false));
}

Point_2 findCollisionZoneEnd(TrajectoryConstructionData& data,
	const Collision& coll)
{
	switch (coll.getType()) {
		case Collision::COLL_WALL: {
			if (CGAL::angle(coll.wall().vector(), data.playerMovementVector)
				== CGAL::ACUTE)
			{
				return coll.wall().pEnd();
			} else {
				return coll.wall().pStart();
			}
		} break;
		case Collision::COLL_CORNER: {
			auto v = data.playerMovementVector.perpendicular(CGAL::CLOCKWISE);
			Line_2 diameterLine(coll.corner().center(), v);
			auto isec = bounds_intersection(coll.corner().circ, diameterLine);
			assert(isec);
			auto isecPts = boost::get<std::vector<Point_2>>(&*isec);
			assert(isecPts);
			// Diameter is a secant, so there have to be 2 intersections
			assert(isecPts->size() == 2);
			if (CGAL::angle(coll.collPoint(), coll.corner().center(),
				isecPts->at(0)) == CGAL::ACUTE)
			{
				return isecPts->at(0);
			}
			else {
				assert(CGAL::angle(coll.collPoint(), coll.corner().center(),
					isecPts->at(1)) == CGAL::ACUTE);
				return isecPts->at(1);
			}
		} break;
	}
	
	// Default
	assert(((void)"Invalid collision type", false));
}

Collision shortestPath(TrajectoryConstructionData& data,
	const TrajectorySegment& trajSeg, const std::vector<Collision>& collisions)
{
	(void)data;

	size_t shortestPathIdx = 0;
	auto shortestPathSq = CGAL::squared_distance(trajSeg.getPStart(),
		collisions.at(shortestPathIdx).collPoint());
	for (size_t i = 1; i < collisions.size(); i++) {
		// If "TrajSeg" is a line segment, it is equal to Euclidean distance between
		// the trajectory source point and the collision point (squared Euclidean
		// distance may be used too). If "TrajSeg" is a circular arc, the Euclidean
		// distance (or squared Euclidean distance) may be used for comparison, unless
		// the length of the arc is longer than `pi * r`. This is always true; the arc
		// length is always within the interval `[0, pi/2 * r)`.
		if (CGAL::squared_distance(trajSeg.getPStart(), collisions.at(i).collPoint())
			< shortestPathSq)
		{
			shortestPathIdx = i;
		}
	}

	return collisions.at(shortestPathIdx);
}

#endif // OLD_TRAJECTORY_ALGORITHM
