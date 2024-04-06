/**
 * @file OneStepLookaheadAIPlayerAgentBase.cpp
 * @author Tomáš Ludrovan
 * @brief OneStepLookaheadAIPlayerAgentBase class
 * @version 0.1
 * @date 2024-03-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "aiplayeragent/OneStepLookaheadAIPlayerAgentBase.hpp"

double OneStepLookaheadAIPlayerAgentBase::evaluatePosition(
	const Point_2& pos) const
{
	double res = 0.0;

	for (const auto& [id, player] : gsProxy->getPlayers()) {
		// Don't evaluate yourself
		if (id == myId) continue;

		res += evaluatePlayer(player, pos);
	}

	return res;
}

void OneStepLookaheadAIPlayerAgentBase::doPlan()
{
	// Possible inputs
	auto inputs = generateInputs();

	// Best evaluation of all inputs
	double bestEval = -std::numeric_limits<double>::infinity();
	// Index of the best evaluated input
	size_t bestEvalIdx = 0;
	
	Point_2 newPos;
	double eval;

	for (size_t i = 0; i < inputs.size(); ++i) {
		newPos = calculateNewPosition(inputs[i]);

		eval = evaluatePosition(newPos);
		if (eval > bestEval) {
			// Found a better input

			bestEval = eval;
			bestEvalIdx = i;
		}
	}

	playerInput = inputs[bestEvalIdx];
}

PlayerInputFlags OneStepLookaheadAIPlayerAgentBase::doGetPlayerInput()
{
	return playerInput;
}

Point_2 OneStepLookaheadAIPlayerAgentBase::calculateNewPositionBlind(
	const PlayerInputFlags& input) const
{
	const auto& myState = getMyState();

	double vx, vy;
	input.toVector(vx, vy);
	auto res = myState.pos + Vector_2(vx, vy);
	return res;
}

Point_2 OneStepLookaheadAIPlayerAgentBase::calculateNewPositionWallAware(
	const PlayerInputFlags& input) const
{
	const auto& myState = getMyState();

	double vx, vy;
	input.toVector(vx, vy);
	Vector_2 playerMove(vx, vy);

	auto traj = gsProxy->getObstacles().getPlayerTrajectory(myState.pos,
		playerMove, myState.size);
	
	return traj.end();
}

OneStepLookaheadAIPlayerAgentBase::OneStepLookaheadAIPlayerAgentBase(
	PlayerId playerId)
	: AIPlayerAgentBase(playerId)
{}
