/**
 * @file WallAwarePredatorAIPlayerAgent.cpp
 * @author Tomáš Ludrovan
 * @brief WallAwarePredatorAIPlayerAgent class
 * @version 0.1
 * @date 2024-03-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "aiplayeragent/WallAwarePredatorAIPlayerAgent.hpp"

Point_2 WallAwarePredatorAIPlayerAgent::calculateNewPosition(
	const PlayerInputFlags& input) const
{
	return calculateNewPositionWallAware(input);
}

double WallAwarePredatorAIPlayerAgent::evaluatePlayer(
	const GameStateAgentProxy::PlayerState& player, const Point_2& pos) const
{
	return PredatorAIPlayerAgentBase::evaluatePlayer(player, pos);
}

WallAwarePredatorAIPlayerAgent::WallAwarePredatorAIPlayerAgent(
	PlayerId playerId)
	: AIPlayerAgentBase(playerId)
	, OneStepLookaheadAIPlayerAgentBase(playerId)
	, PredatorAIPlayerAgentBase(playerId)
{}
