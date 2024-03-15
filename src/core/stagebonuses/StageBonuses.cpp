/**
 * @file StageBonuses.cpp
 * @author Tomáš Ludrovan
 * @brief StageBonuses class
 * @version 0.1
 * @date 2024-03-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "core/stagebonuses/StageBonuses.hpp"

#include "functions.hpp"

#include <ostream>

BonusId StageBonuses::generateBonusId() const
{
	static BonusId lastBonusId = 0;
	return ++lastBonusId;
}

void StageBonuses::initGridOffsets()
{
	m_gridOffsetX = generateGridOffset();
	m_gridOffsetY = generateGridOffset();
}

void StageBonuses::initBonusGrid(const Size2d& stageSize)
{
	RectF stageBounds(BONUS_RADIUS, BONUS_RADIUS,
		stageSize.w - (2*BONUS_RADIUS), stageSize.h - (2*BONUS_RADIUS));
	stageBounds = snapRectangle(stageBounds);
	PointF pt;

	for (pt.y = stageBounds.y; pt.y <= stageBounds.getBottom();
		pt.y += BONUS_GRID_CELL_SIZE) // For each row
	{
		for (pt.x = stageBounds.x; pt.x <= stageBounds.getRight();
			pt.x += BONUS_GRID_CELL_SIZE) // For each column
		{
			m_validPositions.insert(pt);
		}
	}
}

void StageBonuses::invalidatePositionsByObstacles(
	const std::vector<StageObstacle>& obstacles)
{
	// The idea is for each obstacle to find the smallest rectangle in which
	// a collision with bonus may happen, and then for each "valid" position
	// within that rectangle to check if its distance to the obstacle is
	// smaller than the bonus radius, thus would actually cause a collision.

	RectF bbox;
	PointF pt;

	for (const auto& obstacle: obstacles) {
		// Bounding box of the obstacle
		bbox = obstacle.getBoundingBox();
		// Box where a collision may happen
		bbox = bbox.getInflated(2*BONUS_RADIUS);
		// Positions where a collision may happen
		bbox = snapRectangle(bbox);

		for (pt.y = bbox.y; pt.y <= bbox.getBottom();
			pt.y += BONUS_GRID_CELL_SIZE) // For each row
		{
			for (pt.x = bbox.x; pt.x <= bbox.getRight();
				pt.x += BONUS_GRID_CELL_SIZE) // For each column
			{
				if (obstacle.sqrDistance(pt) < sqr(BONUS_RADIUS)) {
					// Causes collision => not valid

					m_validPositions.erase(pt);
				}
			}
		}
	}
}

void StageBonuses::initValidPositions(
	const std::vector<StageObstacle>& obstacles, const Size2d& stageSize)
{
	initGridOffsets();
	initBonusGrid(stageSize);
	invalidatePositionsByObstacles(obstacles);
}

#ifdef ENABLE_BONUS_CONSTRAINTS
void StageBonuses::invalidateCircle(const PointF& center, double radius,
	PointFSet& invalidatedOut)
{
	// Inspired from Bresenham's midpoint circle algorithm. Instead of
	// invalidating coordinates, we can imagine drawing a filled circle.
	// We won't separate the circle to 8 octants, because they are not
	// symmetrical; instead we will paint just one half. To "fill" the
	// circle instead of merely "stroking" it, we won't be drawing only one
	// point per each loop iteration, but all points horizontally up to the
	// vertical line passing through the center of the circle.

	// First, find the extrema (the leftmost, topmost, rightmost, and bottommost
	// valid position coordinates within the provided circle).
	RectF extrema(center.x - radius, center.y - radius, 2*radius, 2*radius);
	extrema = snapRectangle(extrema);

	// Snap center
	PointF centerSnap = snapPoint(center);

	PointF borderPt, brushPt;
	bool isQuadrantFilled;

	// Quadrant 1 (top right)

	//   |
	// --+-O
	//   |
	borderPt = PointF(extrema.getRight(), centerSnap.y);
	isQuadrantFilled = false;
	while (!isQuadrantFilled) {
		// Must be within the circle
		while (borderPt.sqrDistance(center) > sqr(radius)) {
			borderPt.x -= BONUS_GRID_CELL_SIZE; // Move left
			if (borderPt.x < center.x) { // Left of the "vertical central
			                             // secant"
				isQuadrantFilled = true;
				break;
			}
		}
		// Paint
		brushPt = borderPt;
		while (brushPt.x >= center.x) { // Right of or at the "vertical central
		                                // secant"
			m_validPositions.erase(brushPt);
			invalidatedOut.insert(brushPt);
			brushPt.x -= BONUS_GRID_CELL_SIZE; // Move left
		}
		borderPt.y -= BONUS_GRID_CELL_SIZE; // Move up
	}

	// Quadrant 2 (top left)
	
	//   |
	// O-+--
	//   |
	borderPt = PointF(extrema.x, centerSnap.y);
	while (borderPt.y >= extrema.y) {
		// Must be within the circle
		while (borderPt.sqrDistance(center) > sqr(radius)) {
			borderPt.x += BONUS_GRID_CELL_SIZE; // Move right
			if (borderPt.x > center.x) { // Right of the "vertical central
			                             // secant"
				isQuadrantFilled = true;
				break;
			}
		}
		// Paint
		brushPt = borderPt;
		while (brushPt.x < center.x) { // Left of the "vertical central secant"
			m_validPositions.erase(brushPt);
			invalidatedOut.insert(brushPt);
			brushPt.x += BONUS_GRID_CELL_SIZE; // Move right
		}
		borderPt.y -= BONUS_GRID_CELL_SIZE; // Move up
	}

	// Quadrant 3 (bottom left)
	
	//   |
	// .-+--
	//   |
	// Starts a little lower, because the row above is already painted
	borderPt = PointF(extrema.x, centerSnap.y + BONUS_GRID_CELL_SIZE);
	while (borderPt.y <= extrema.getBottom()) {
		// Must be within the circle
		while (borderPt.sqrDistance(center) > sqr(radius)) {
			borderPt.x += BONUS_GRID_CELL_SIZE; // Move right
			if (borderPt.x > center.x) { // Right of the "vertical central
			                             // secant"
				isQuadrantFilled = true;
				break;
			}
		}
		// Paint
		brushPt = borderPt;
		while (brushPt.x < center.x) { // Left of the "vertical central secant"
			m_validPositions.erase(brushPt);
			invalidatedOut.insert(brushPt);
			brushPt.x += BONUS_GRID_CELL_SIZE; // Move right
		}
		borderPt.y += BONUS_GRID_CELL_SIZE; // Move down
	}

	// Quadrant 4 (bottom right)

	//   |
	// --+-.
	//   |
	// Starts a little lower, because the row above is already painted
	borderPt = PointF(extrema.getRight(), centerSnap.y + BONUS_GRID_CELL_SIZE);
	while (borderPt.y <= extrema.getBottom()) {
		// Must be within the circle
		while (borderPt.sqrDistance(center) > sqr(radius)) {
			borderPt.x -= BONUS_GRID_CELL_SIZE; // Move left
			if (borderPt.x < center.x) { // Left of the "vertical central
			                             // secant"
				isQuadrantFilled = true;
				break;
			}
		}
		// Paint
		brushPt = borderPt;
		while (brushPt.x >= center.x) { // Right of or at the "vertical central
		                                // secant"
			m_validPositions.erase(brushPt);
			invalidatedOut.insert(brushPt);
			brushPt.x -= BONUS_GRID_CELL_SIZE; // Move left
		}
		borderPt.y += BONUS_GRID_CELL_SIZE; // Move down
	}
}
#endif // ENABLE_BONUS_CONSTRAINTS

RectF StageBonuses::snapRectangle(const RectF& rect) const
{
	RectF res = rect;
	// Remove offset
	res.offset(-m_gridOffsetX, -m_gridOffsetY);
	// Round
	// +------------------------------------>
	// |            || TOP               +x
	// |            \/ roundUp
	// | LEFT    +-------------+ RIGHT
	// | roundUp | +    +    + | roundDown
	// |     ==> |             | <==
	// |         | +    +    + |
	// |         +-------------+
	// |  +y        /\ BOTTOM
	// V            || roundDown
	double l = roundfUpToMultiple(rect.x, BONUS_GRID_CELL_SIZE);
	double t = roundfUpToMultiple(rect.y, BONUS_GRID_CELL_SIZE);
	double r = roundfDownToMultiple(rect.getRight(), BONUS_GRID_CELL_SIZE);
	double b = roundfDownToMultiple(rect.getBottom(), BONUS_GRID_CELL_SIZE);
	res = RectF(PointF(l, t), PointF(r, b));
	// Add offset
	res.offset(m_gridOffsetX, m_gridOffsetY);

	return res;
}

PointF StageBonuses::snapPoint(const PointF& pt) const
{
	// Remove offset
	PointF res(pt.x - m_gridOffsetX, pt.y - m_gridOffsetY);
	// Round
	res.x = roundfToMultiple(res.x, BONUS_GRID_CELL_SIZE);
	res.y = roundfToMultiple(res.y, BONUS_GRID_CELL_SIZE);
	// Add offset
	res.x += m_gridOffsetX;
	res.y += m_gridOffsetY;

	return res;
}

double StageBonuses::generateGridOffset()
{
	// The number of binary digits in the fractional part of the generated
	// number. The value should not be too high in order to represent big
	// enough numbers. (Well not that big, about 2^16 should be enough, but
	// the `DECIMAL_PLACES` doesn't have to be too high either.)
	static constexpr int DECIMAL_PLACES = 4;
	// The number of unique fractional parts of the result
	static constexpr int FRACTION_COUNT = (1 << DECIMAL_PLACES);
	// The number of unique results
	static constexpr int VALUE_COUNT =
		static_cast<int>(BONUS_GRID_CELL_SIZE) * FRACTION_COUNT;

	int r = rand() % VALUE_COUNT;
	double res = static_cast<double>(r) / static_cast<double>(FRACTION_COUNT);
	return res;
}

double StageBonuses::generateHpRecovery()
{
	//  x   | 0.25 | 0.50 | 0.75 | 1.00
	// p(x) | 2/8  | 3/8  | 2/8  | 1/8
	// F(x) | 2/8  | 5/8  | 7/8  | 8/8

	static std::uniform_int_distribution<int> distrib(0, 7);
	int Fx = distrib(getRNGine());

	if (Fx < 2)
		return 0.25;
	else if (Fx < 5)
		return 0.5;
	else if (Fx < 7)
		return 0.75;
	else
		return 1.0;
}

StageBonuses::StageBonuses(const std::vector<StageObstacle>& obstacles,
	const Size2d& stageSize)
{
	initValidPositions(obstacles, stageSize);
}

#ifdef ENABLE_BONUS_CONSTRAINTS
void StageBonuses::reportPlayerStates(const std::vector<PlayerState>& states)
{
	for (const auto& pt : m_playerInvalidPositions) {
		if (m_bonusInvalidPositions.find(pt)
			== m_bonusInvalidPositions.cend())
		{
			m_validPositions.insert(pt);
		}
	}
	m_playerInvalidPositions.clear();

	for (const auto& state : states) {
		invalidateCircle(PointF(state.x, state.y),
			state.size + BONUS_RADIUS + BONUS_PLAYER_MIN_DISTANCE,
			m_playerInvalidPositions);
	}
}
#endif // ENABLE_BONUS_CONSTRAINTS

BonusId StageBonuses::generateBonus()
{
	if (m_validPositions.size() == 0) {
		// No valid positions => cannot generate bonus

		return BONUS_ID_NULL;
	} else {
		// Bonus ID
		BonusId id = generateBonusId();

		// Choose position
		std::uniform_int_distribution<size_t> distrib(0,
			m_validPositions.size() - 1);
		size_t posIdx = distrib(getRNGine());
		PointF position = m_validPositions.atIndex(static_cast<size_t>(posIdx));

		// Choose HP recovery
		double hpRecovery = generateHpRecovery();
		
		// Create
		m_bonuses[id] = BonusData(position, hpRecovery);

#ifdef ENABLE_BONUS_CONSTRAINTS
		invalidateCircle(pt, 2*BONUS_RADIUS, m_bonusInvalidPositions);
#endif

		return id;
	}
}

void StageBonuses::clearBonus(BonusId id)
{
	m_bonuses.erase(id);
	(void)id;
	// TODO
}

const std::unordered_map<BonusId, StageBonuses::BonusData>&
StageBonuses::getBonuses() const
{
	return m_bonuses;
}
