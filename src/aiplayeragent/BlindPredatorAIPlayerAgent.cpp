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

Point_2 BlindPredatorAIPlayerAgent::calculateNewPosition(
	const PlayerInputFlags& input) const
{
	const auto& me = gsProxy->getPlayers().at(playerId);

	double vx, vy;
	input.toVector(vx, vy);
	auto res = me.pos + Vector_2(vx, vy);
	return res;
}

double BlindPredatorAIPlayerAgent::evaluatePlayer(
	const GameStateAgentProxy::PlayerState& player, const Point_2& pos) const
{
	double sqdist = CGAL::squared_distance(pos, player.pos);
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

BlindPredatorAIPlayerAgent::BlindPredatorAIPlayerAgent(PlayerId playerId)
	: OneStepLookaheadAIPlayerAgentBase(playerId)
{}
