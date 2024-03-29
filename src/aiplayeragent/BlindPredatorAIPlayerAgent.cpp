/**
 * @file BlindPredatorAIPlayerAgent.cpp
 * @author Tomáš Ludrovan
 * @brief BlindPredatorAIPlayerAgent class
 * @version 0.1
 * @date 2024-03-26
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "aiplayeragent/BlindPredatorAIPlayerAgent.hpp"

std::vector<PlayerInputFlags> BlindPredatorAIPlayerAgent::generateInputs()
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

double BlindPredatorAIPlayerAgent::evaluatePosition(const PointF& pos) const
{
	double res = 0.0;

	for (const auto& [id, player] : gsProxy->getPlayers()) {
		// Don't evaluate yourself
		if (id == playerId) continue;

		res += evaluatePlayer(player, pos);
	}

	return res;
}

double BlindPredatorAIPlayerAgent::evaluatePlayer(
	const GameStateAgentProxy::PlayerState& player, const PointF& pos)
{
	double sqdist = pos.sqrDistance(player.pos);
	double hp = player.hp;

	// Player HP significance
	static constexpr double HP_SIGNIF = 10.0;
	// Player squared distance significance
	static constexpr double SQDIST_SIGNIF = 30.0;

	// Weaker players are evaluated better than stronger ones
	double hpEval = HP_SIGNIF / hp;
	// Closer players are evaluated better than farther ones
	double sqdistEval = SQDIST_SIGNIF / sqdist;

	return hpEval + sqdistEval;
}

void BlindPredatorAIPlayerAgent::doPlan()
{
	// This agent state
	const auto& me = gsProxy->getPlayers().at(playerId);

	// Possible inputs
	auto inputs = generateInputs();

	// Best evaluation of all inputs
	double bestEval = -std::numeric_limits<double>::infinity();
	// Index of the best evaluated input
	size_t bestEvalIdx = 0;
	
	PointF mvVect;
	double eval;

	for (size_t i = 0; i < inputs.size(); ++i) {
		// Calculate the movement vector for the given input
		inputs[i].toVector(mvVect.x, mvVect.y);

		eval = evaluatePosition(me.pos + mvVect);
		if (eval > bestEval) {
			// Found a better input

			bestEval = eval;
			bestEvalIdx = i;
		}
	}

	m_input = inputs[bestEvalIdx];
}

PlayerInputFlags BlindPredatorAIPlayerAgent::doGetPlayerInput()
{
	return m_input;
}

BlindPredatorAIPlayerAgent::BlindPredatorAIPlayerAgent(PlayerId playerId)
	: AIPlayerAgentBase(playerId)
{}
