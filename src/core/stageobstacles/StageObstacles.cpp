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

void StageObstacles::inputToVector(const PlayerInputFlags& input, double& x, double& y)
{
	// If none or both keys for horizontal movement are pressed, the player
	// won't move horizontally
	bool isHorzMove = (input.left != input.right);
	// Same as isHorzMove
	bool isVertMove = (input.up != input.down);

	if (!isHorzMove && !isVertMove) {
		// Doesn't move

		x = 0.0;
		y = 0.0;
	} else if (isHorzMove && isVertMove) {
		// Diagonal

		x = (input.left ? -DIAG_MOVEMENT_PERAXIS_LENGTH : DIAG_MOVEMENT_PERAXIS_LENGTH);
		y = (input.up ? -DIAG_MOVEMENT_PERAXIS_LENGTH : DIAG_MOVEMENT_PERAXIS_LENGTH);
	} else {
		// Orthogonal
		
		if (isHorzMove) {
			x = (input.left ? -1.0 : 1.0);
			y = 0.0;
		} else {
			x = 0.0;
			y = (input.up ? -1.0 : 1.0);
		}
	}
}

StageObstacles::StageObstacles(
	const std::vector<StageObstacle>& obstacles, const Size2d& bounds)
	: m_obstacles{obstacles}
	, m_bounds{bounds}
{}

const std::vector<StageObstacle>& StageObstacles::getObstaclesList() const
{
	return m_obstacles;
}

const Size2d& StageObstacles::getStageSize() const
{
	return m_bounds;
}

void StageObstacles::getPlayerTrajectory(std::shared_ptr<IPlayerState> playerState,
	Trajectory& trajectory)
{
	double ux, uy;
	inputToVector(playerState->getPlayerInputObject()->readInput(), ux, uy);

	trajectory.segment.lineSegment.Ax = playerState->getX();
	trajectory.segment.lineSegment.Ay = playerState->getY();
	trajectory.segment.lineSegment.ux = ux * playerState->getSpeed() * SPEED_FACTOR;
	trajectory.segment.lineSegment.uy = uy * playerState->getSpeed() * SPEED_FACTOR;
}
