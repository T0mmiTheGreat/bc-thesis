/**
 * @file PlayerInputFactory.cpp
 * @author Tomáš Ludrovan
 * @brief PlayerInputFactory class
 * @version 0.1
 * @date 2023-12-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "playerinput/PlayerInputFactory.hpp"

#include "playerinput/KeyboardPlayerInput.hpp"
#include "playerinput/ImmobilePlayerInput.hpp"
#include "playerinput/AIPlayerInput.hpp"

std::shared_ptr<IPlayerInput> PlayerInputFactory::createKeyboardPlayerInput(
	std::shared_ptr<ISysProxyPlayerInput> sysProxy, KeyCode keyLeft,
	KeyCode keyUp, KeyCode keyRight, KeyCode keyDown)
{
	return std::make_shared<KeyboardPlayerInput>(sysProxy, keyLeft, keyUp,
		keyRight, keyDown);
}

std::shared_ptr<IPlayerInput> PlayerInputFactory::createKeyboardPlayerInputWSAD(
	std::shared_ptr<ISysProxyPlayerInput> sysProxy)
{
	return std::make_shared<KeyboardPlayerInput>(
		KeyboardPlayerInput::constructWSAD(sysProxy));
}

std::shared_ptr<IPlayerInput> PlayerInputFactory::createKeyboardPlayerInputIKJL(
	std::shared_ptr<ISysProxyPlayerInput> sysProxy)
{
	return std::make_shared<KeyboardPlayerInput>(
		KeyboardPlayerInput::constructIKJL(sysProxy));
}

std::shared_ptr<IPlayerInput> PlayerInputFactory::createKeyboardPlayerInputArrows(
	std::shared_ptr<ISysProxyPlayerInput> sysProxy)
{
	return std::make_shared<KeyboardPlayerInput>(
		KeyboardPlayerInput::constructArrows(sysProxy));
}

std::shared_ptr<IPlayerInput> PlayerInputFactory::createImmobilePlayerInput()
{
	return std::make_shared<ImmobilePlayerInput>();
}

std::shared_ptr<IPlayerInput> PlayerInputFactory::createAIPlayerInput()
{
	return std::make_shared<AIPlayerInput>();
}
