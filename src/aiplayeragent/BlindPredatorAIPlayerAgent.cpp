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
	return calculateNewPositionBlind(input);
}

double BlindPredatorAIPlayerAgent::evaluatePlayer(
	const GameStateAgentProxy::PlayerState& player, const Point_2& pos) const
{
	return PredatorAIPlayerAgentBase::evaluatePlayer(player, pos);
}

BlindPredatorAIPlayerAgent::BlindPredatorAIPlayerAgent(PlayerId playerId)
	: AIPlayerAgentBase(playerId)
	, OneStepLookaheadAIPlayerAgentBase(playerId)
	, PredatorAIPlayerAgentBase(playerId)
{}
