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

KeyboardPlayerInput::KeyboardPlayerInput(
	std::shared_ptr<ISysProxyPlayerInput> sysProxy, KeyCode keyLeft,
	KeyCode keyUp, KeyCode keyRight, KeyCode keyDown)
	: m_keyLeft{keyLeft}
	, m_keyUp{keyUp}
	, m_keyRight{keyRight}
	, m_keyDown{keyDown}
	, m_sysProxy{sysProxy}
{}

KeyboardPlayerInput KeyboardPlayerInput::constructWSAD(
	std::shared_ptr<ISysProxyPlayerInput> sysProxy)
{
	return KeyboardPlayerInput(sysProxy, KEY_A, KEY_W, KEY_D, KEY_S);
}

KeyboardPlayerInput KeyboardPlayerInput::constructArrows(
	std::shared_ptr<ISysProxyPlayerInput> sysProxy)
{
	return KeyboardPlayerInput(sysProxy, KEY_LEFT_ARROW, KEY_UP_ARROW,
		KEY_RIGHT_ARROW, KEY_DOWN_ARROW);
}

PlayerInputState KeyboardPlayerInput::readInput()
{
	PlayerInputState res;
	
	res.left = m_sysProxy->isKeyPressed(m_keyLeft);
	res.up = m_sysProxy->isKeyPressed(m_keyUp);
	res.right = m_sysProxy->isKeyPressed(m_keyRight);
	res.down = m_sysProxy->isKeyPressed(m_keyDown);

	return res;
}
