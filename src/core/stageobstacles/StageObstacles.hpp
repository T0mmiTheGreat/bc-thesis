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

#include "core/Constants.hpp"
#include "core/trajectory/Trajectory.hpp"
#include "playerinput/IPlayerInput.hpp"
#include "playerstate/IPlayerState.hpp"

class StageObstacles {
// private:
// 	struct Obstacle {
// 		double x1, y1, x2, y2, x3, y3;
// 	};
private:
// 	std::vector<Obstacle> m_obstacles;

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
	/**
	 * @brief Returns the player trajectory based on the player input, taking
	 *        the obstacles into account.
	 */
	void getPlayerTrajectory(std::shared_ptr<IPlayerState> playerState,
		Trajectory& trajectory);
};

#endif // STAGEOBSTACLES_HPP
