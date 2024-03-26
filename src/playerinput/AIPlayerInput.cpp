/**
 * @file AIPlayerInput.cpp
 * @author Tomáš Ludrovan
 * @brief AIPlayerInput class
 * @version 0.1
 * @date 2024-02-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "playerinput/AIPlayerInput.hpp"

#include <cstdlib>

AIPlayerInput::AIPlayerInput(std::shared_ptr<IAIPlayerAgent> agent)
	: m_agent{agent}
{}

PlayerInputFlags AIPlayerInput::readInput()
{
	return m_agent->getPlayerInput();
}
