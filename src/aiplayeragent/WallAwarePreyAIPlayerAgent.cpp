/**
 * @file WallAwarePreyAIPlayerAgent.cpp
 * @author Tomáš Ludrovan
 * @brief WallAwarePreyAIPlayerAgent class
 * @version 0.1
 * @date 2024-03-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "aiplayeragent/WallAwarePreyAIPlayerAgent.hpp"

Point_2 WallAwarePreyAIPlayerAgent::calculateNewPosition(
	const PlayerInputFlags& input) const
{
	return calculateNewPositionWallAware(input);
}

double WallAwarePreyAIPlayerAgent::evaluatePlayer(
	const GameStateAgentProxy::PlayerState& player, const Point_2& pos) const
{
	return evaluatePlayerPrey(player, pos);
}

WallAwarePreyAIPlayerAgent::WallAwarePreyAIPlayerAgent(PlayerId playerId)
	: OneStepLookaheadAIPlayerAgentBase(playerId)
{}
