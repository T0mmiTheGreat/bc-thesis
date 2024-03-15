/**
 * @file StageObstacles.hpp
 * @author Tomáš Ludrovan
 * @brief StageObstacles class
 * @version 0.1
 * @date 2024-01-14
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef STAGEOBSTACLES_HPP
#define STAGEOBSTACLES_HPP

#include <memory>
#include <vector>

#include "types.hpp"
#include "core/Common.hpp"
#include "core/geometry/Geometry.hpp"
#include "core/trajectory/Trajectory.hpp"
#include "playerinput/IPlayerInput.hpp"

#ifdef OLD_TRAJECTORY_ALGORITHM
class StageObstacles {
private:
	struct Wall {
		// Line segment representing the wall
		Segment_2 seg;
		// Precomputed normal unit vector
		Vector_2 n;
	};
	struct Corner {
		Point_2 p;
	};
private:
	// These are kept just because the controller will query them, but this
	// class doesn't use these variables. (Only once, to initialize other
	// things.)

 	std::vector<StageObstacle> m_obstacles;
	Size2d m_bounds;

	std::vector<Wall> m_walls;
	std::vector<Corner> m_corners;

	/**
	 * @brief Initializes the wall and corner data.
	 * 
	 * @param obstacles List of obstacles.
	 * @param bounds Stage bounds.
	 */
	void initializeCollisionObjects(const std::vector<StageObstacle>& obstacles,
		const Size2d& bounds);
	/**
	 * @brief Fills the walls list.
	 * 
	 * @param obstacles List of obstacles.
	 * @param bounds Stage bounds.
	 */
	void initializeWalls(const std::vector<StageObstacle>& obstacles,
		const Size2d& bounds);
	/**
	 * @brief Fills the corners list.
	 * 
	 * @param obstacles List of obstacles.
	 * @param bounds Stage bounds.
	 */
	void initializeCorners(const std::vector<StageObstacle>& obstacles,
		const Size2d& bounds);

	/**
	 * @brief Calculates the unit normal vector of a wall.
	 * 
	 * @param wallSeg Line segment representing the wall.
	 */
	Vector_2 getWallUnitNormal(const Segment_2& wallSeg);

	/**
	 * @brief Adds walls of the `obstacle` to the walls list.
	 */
	void addObstacleToWallsList(const StageObstacle& obstacle);
	/**
	 * @brief Adds the stage walls to the walls list.
	 */
	void addBoundsToWallsList(const Size2d& bounds);
	/**
	 * @brief Adds corners of the `obstacle` to the corners list.
	 */
	void addObstacleToCornersList(const StageObstacle& obstacle);
	/**
	 * @brief Adds the stage corners to the corners list.
	 */
	void addBoundsToCornersList(const Size2d& bounds);

	/**
	 * @brief Creates a wall object and adds it to the walls list.
	 * 
	 * @param sp Source point of the wall.
	 * @param ep Target (end) point of the wall.
	 */
	void addWallToList(const Point_2& sp, const Point_2& ep);
	/**
	 * @brief Creates a wall object and adds it to the walls list.
	 * 
	 * @param seg Line segment representing the wall.
	 */
	void addWallToList(const Segment_2& seg);
	/**
	 * @brief Creates a corner object and adds it to the corners list.
	 * 
	 * @param p Point representing the corner.
	 */
	void addCornerToList(const Point_2& p);
public:
	StageObstacles(const std::vector<StageObstacle>& obstacles,
		const Size2d& bounds);
	const std::vector<StageObstacle>& getObstaclesList() const;
	const Size2d& getStageSize() const;
	/**
	 * @brief Creates the player trajectory, taking the obstacles into account.
	 */
	Trajectory getPlayerTrajectory(const Point_2& playerPos,
		const Vector_2& playerMove, double playerRadius);
};
#else // OLD_TRAJECTORY_ALGORITHM
class StageObstacles {
private:
	// Collision objects
	std::vector<Triangle_2> m_collObjs;

	/**
	 * @brief Initializes the collision objects data.
	 * 
	 * @param obstacles List of obstacles.
	 * @param bounds Stage bounds.
	 */
	void initializeCollisionObjects(const std::vector<StageObstacle>& obstacles,
		const Size2d& bounds);

	/**
	 * @brief Adds the `obstacle` to the collision objects list.
	 */
	void addObstacleToCollisionObjects(const StageObstacle& obstacle);
	/**
	 * @brief Adds the stage walls to the collision objects list.
	 */
	void addBoundsToCollisionObjects(const Size2d& bounds);
public:
	StageObstacles(const std::vector<StageObstacle>& obstacles,
		const Size2d& bounds);
	/**
	 * @brief Creates the player trajectory, taking the obstacles into account.
	 */
	Trajectory getPlayerTrajectory(const Point_2& playerPos,
		const Vector_2& playerMove, double playerRadius);
	/**
	 * @brief Checks whether a player at `playerPos` position having
	 *        `playerRadius` radius collides with any collision object.
	 */
	bool playerHasCollision(const Point_2& playerPos, double playerRadius);
};
#endif // !OLD_TRAJECTORY_ALGORITHM

#endif // STAGEOBSTACLES_HPP
