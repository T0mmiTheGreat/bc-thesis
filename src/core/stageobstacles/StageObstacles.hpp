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
#include "core/trajectory/Trajectory.hpp"
#include "playerinput/IPlayerInput.hpp"
#include "playerstate/IPlayerState.hpp"

class StageObstacles {
private:
 	std::vector<StageObstacle> m_obstacles;
	Size2d m_bounds;

	// sin(pi/4)
	static constexpr double DIAG_MOVEMENT_PERAXIS_LENGTH = 0.70710678118654752440084436210485;
	// Value to multiply with the player speed
	static constexpr double SPEED_FACTOR = TICK_INTERVAL / 6.0;
	/**
	 * @brief Convert player input value to a vector.
	 * 
	 * @details Based on the player input creates a unit vector in the direction
	 *          the player wants to move.
	 * 
	 * @param input The player input.
	 * @param x Vector X coordinate.
	 * @param y Vector Y coordinate.
	 */
	static void inputToVector(const PlayerInputFlags& input, double& x, double& y);
public:
	StageObstacles(const std::vector<StageObstacle>& obstacles,
		const Size2d& bounds);
	const std::vector<StageObstacle>& getObstaclesList() const;
	const Size2d& getStageSize() const;
	/**
	 * @brief Modifies the player trajectory by taking the obstacles into
	 *        account.
	 */
	void adjustPlayerTrajectory(Trajectory& trajectory, double playerRadius);
};

#endif // STAGEOBSTACLES_HPP
