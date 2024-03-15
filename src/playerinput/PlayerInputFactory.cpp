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
	KeyCode keyUp, KeyCode keyRight, KeyCode keyDown, KeyCode keyDeflate)
{
	return std::make_shared<KeyboardPlayerInput>(sysProxy, keyLeft, keyUp,
		keyRight, keyDown, keyDeflate);
}

std::shared_ptr<IPlayerInput> PlayerInputFactory::createKeyboardPlayerInputWSADQ(
	std::shared_ptr<ISysProxyPlayerInput> sysProxy)
{
	return std::make_shared<KeyboardPlayerInput>(
		KeyboardPlayerInput::constructWSADQ(sysProxy));
}

std::shared_ptr<IPlayerInput> PlayerInputFactory::createKeyboardPlayerInputIKJLU(
	std::shared_ptr<ISysProxyPlayerInput> sysProxy)
{
	return std::make_shared<KeyboardPlayerInput>(
		KeyboardPlayerInput::constructIKJLU(sysProxy));
}

std::shared_ptr<IPlayerInput> PlayerInputFactory::createKeyboardPlayerInputArrowsShift(
	std::shared_ptr<ISysProxyPlayerInput> sysProxy)
{
	return std::make_shared<KeyboardPlayerInput>(
		KeyboardPlayerInput::constructArrowsShift(sysProxy));
}

std::shared_ptr<IPlayerInput> PlayerInputFactory::createImmobilePlayerInput()
{
	return std::make_shared<ImmobilePlayerInput>();
}

std::shared_ptr<IPlayerInput> PlayerInputFactory::createAIPlayerInput()
{
	return std::make_shared<AIPlayerInput>();
}
