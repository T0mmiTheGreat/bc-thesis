/**
 * @file LadybugAIPlayerAgent.cpp
 * @author Tomáš Ludrovan
 * @brief LadybugAIPlayerAgent class
 * @version 0.1
 * @date 2024-03-26
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "aiplayeragent/LadybugAIPlayerAgent.hpp"

#include <cstdlib>

void LadybugAIPlayerAgent::doPlan()
{
	auto randVal = rand();
	if ((randVal % 256) < (1 << 4)) {
		// Try new input

		m_input.left  = (randVal & (1 << 0));
		m_input.up    = (randVal & (1 << 1));
		m_input.right = (randVal & (1 << 2));
		m_input.down  = (randVal & (1 << 3));
	}
	// Else, keep old input
}

PlayerInputFlags LadybugAIPlayerAgent::doGetPlayerInput()
{
	return m_input;
}

LadybugAIPlayerAgent::LadybugAIPlayerAgent(PlayerId playerId)
	: AIPlayerAgentBase(playerId)
	, m_input()
{}
