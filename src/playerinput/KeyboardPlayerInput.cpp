/**
 * @file KeyboardPlayerInput.cpp
 * @author Tomáš Ludrovan
 * @brief KeyboardPlayerInput class
 * @version 0.1
 * @date 2023-12-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "playerinput/KeyboardPlayerInput.hpp"
#include "sysproxy/SysProxyFactory.hpp"

KeyboardPlayerInput::KeyboardPlayerInput(KeyCode keyLeft, KeyCode keyUp, KeyCode keyRight, KeyCode keyDown)
	: m_keyLeft{keyLeft}
	, m_keyUp{keyUp}
	, m_keyRight{keyRight}
	, m_keyDown{keyDown}
{}

KeyboardPlayerInput KeyboardPlayerInput::constructWSAD()
{
	return KeyboardPlayerInput(KEY_A, KEY_W, KEY_D, KEY_S);
}

KeyboardPlayerInput KeyboardPlayerInput::constructArrows()
{
	return KeyboardPlayerInput(KEY_LEFT_ARROW, KEY_UP_ARROW, KEY_RIGHT_ARROW, KEY_DOWN_ARROW);
}

PlayerInputState KeyboardPlayerInput::readInput()
{
	PlayerInputState res;
	auto proxy = SysProxyFactory::createDefault();
	
	res.left = proxy->isKeyPressed(m_keyLeft);
	res.up = proxy->isKeyPressed(m_keyUp);
	res.right = proxy->isKeyPressed(m_keyRight);
	res.down = proxy->isKeyPressed(m_keyDown);

	return res;
}
