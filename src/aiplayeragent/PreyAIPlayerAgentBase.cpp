/**
 * @file PreyAIPlayerAgentBase.cpp
 * @author Tomáš Ludrovan
 * @brief PreyAIPlayerAgentBase class
 * @version 0.1
 * @date 2024-04-06
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "aiplayeragent/PreyAIPlayerAgentBase.hpp"

const GameStateAgentProxy::PlayerState* PreyAIPlayerAgentBase::chooseAttacker()
{
	Point_2 myPos = getMyState().pos;
	const GameStateAgentProxy::PlayerState* worstAttackerPtr = nullptr;
	double attackerEval = 0.0;
	double worstAttackerEval = std::numeric_limits<double>::infinity();

	for (const auto& [id, player] : gsProxy->getPlayers()) {
		// Don't evaluate yourself
		if (id == myId) continue;

		attackerEval = evaluatePlayer(player, myPos);
		if (attackerEval < worstAttackerEval) {
			// Found a worse attacker

			worstAttackerPtr = &player;
			worstAttackerEval = attackerEval;
		}
	}

	return worstAttackerPtr;
}

double PreyAIPlayerAgentBase::evaluatePlayer(
	const GameStateAgentProxy::PlayerState& player, const Point_2& pos) const
{
	double sqdist = CGAL::squared_distance(pos, player.pos);
	double hp = player.hp;

	// Player HP significance
	static constexpr double HP_SIGNIF = 10.0;
	// Player squared distance significance
	static constexpr double SQDIST_SIGNIF = 30.0;

	// Weaker players are evaluated better than stronger ones
	double hpEval = -HP_SIGNIF * hp;
	// Farther players are evaluated better than closer ones
	double sqdistEval = -SQDIST_SIGNIF / sqdist;

	return hpEval + sqdistEval;
}

PreyAIPlayerAgentBase::PreyAIPlayerAgentBase(PlayerId playerId)
	: AIPlayerAgentBase(playerId)
{}
