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

AIPlayerInput::AIPlayerInput()
{}

PlayerInputFlags AIPlayerInput::readInput()
{
	auto randVal = rand();
	if ((randVal % 256) < (1 << 4)) {
		m_prevMove.left  = (randVal & (1 << 0));
		m_prevMove.up    = (randVal & (1 << 1));
		m_prevMove.right = (randVal & (1 << 2));
		m_prevMove.down  = (randVal & (1 << 3));
	}

	return m_prevMove;
}
