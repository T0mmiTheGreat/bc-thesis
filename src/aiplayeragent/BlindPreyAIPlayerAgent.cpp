/**
 * @file BlindPreyAIPlayerAgent.cpp
 * @author Tomáš Ludrovan
 * @brief BlindPreyAIPlayerAgent class
 * @version 0.1
 * @date 2024-03-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "aiplayeragent/BlindPreyAIPlayerAgent.hpp"

Point_2 BlindPreyAIPlayerAgent::calculateNewPosition(
	const PlayerInputFlags& input) const
{
	return calculateNewPositionBlind(input);
}

double BlindPreyAIPlayerAgent::evaluatePlayer(
	const GameStateAgentProxy::PlayerState& player, const Point_2& pos) const
{
	return evaluatePlayerPrey(player, pos);
}

BlindPreyAIPlayerAgent::BlindPreyAIPlayerAgent(PlayerId playerId)
	: OneStepLookaheadAIPlayerAgentBase(playerId)
{}
