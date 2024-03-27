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

std::vector<PlayerInputFlags>
OneStepLookaheadAIPlayerAgentBase::generateInputs()
{
	static constexpr size_t NUM_INPUTS = 9;

	std::vector<PlayerInputFlags> res;
	res.reserve(NUM_INPUTS);

	res.push_back(PlayerInputFlags());
	res.push_back(PlayerInputFlags::createW());
	res.push_back(PlayerInputFlags::createNW());
	res.push_back(PlayerInputFlags::createN());
	res.push_back(PlayerInputFlags::createNE());
	res.push_back(PlayerInputFlags::createE());
	res.push_back(PlayerInputFlags::createSE());
	res.push_back(PlayerInputFlags::createS());
	res.push_back(PlayerInputFlags::createSW());

	return res;
}

double OneStepLookaheadAIPlayerAgentBase::evaluatePosition(
	const Point_2& pos) const
{
	double res = 0.0;

	for (const auto& [id, player] : gsProxy->getPlayers()) {
		// Don't evaluate yourself
		if (id == playerId) continue;

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

OneStepLookaheadAIPlayerAgentBase::OneStepLookaheadAIPlayerAgentBase(
	PlayerId playerId)
	: AIPlayerAgentBase(playerId)
{}
