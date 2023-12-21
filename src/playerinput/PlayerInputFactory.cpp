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

std::shared_ptr<IPlayerInput> PlayerInputFactory::createKeyboardPlayerInput(
	KeyCode keyLeft, KeyCode keyUp, KeyCode keyRight, KeyCode keyDown)
{
	return std::make_shared<KeyboardPlayerInput>(keyLeft, keyUp, keyRight,
		keyDown);
}

std::shared_ptr<IPlayerInput> PlayerInputFactory::createKeyboardPlayerInputWSAD()
{
	return std::make_shared<KeyboardPlayerInput>(
		KeyboardPlayerInput::constructWSAD());
}

std::shared_ptr<IPlayerInput> PlayerInputFactory::createKeyboardPlayerInputArrows()
{
	return std::make_shared<KeyboardPlayerInput>(
		KeyboardPlayerInput::constructArrows());
}

std::shared_ptr<IPlayerInput> PlayerInputFactory::createImmobilePlayerInput()
{
	return std::make_shared<ImmobilePlayerInput>();
}
