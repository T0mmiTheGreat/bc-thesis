/**
 * @file PlayerInputFactory.hpp
 * @author Tomáš Ludrovan
 * @brief PlayerInputFactory class
 * @version 0.1
 * @date 2023-12-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef PLAYERINPUTFACTORY_HPP
#define PLAYERINPUTFACTORY_HPP

#include <memory>

#include "types.hpp"
#include "playerinput/IPlayerInput.hpp"

class PlayerInputFactory {
public:
	static std::shared_ptr<IPlayerInput> createKeyboardPlayerInput(
		KeyCode keyLeft, KeyCode keyUp, KeyCode keyRight, KeyCode keyDown);
	static std::shared_ptr<IPlayerInput> createKeyboardPlayerInputWSAD();
	static std::shared_ptr<IPlayerInput> createKeyboardPlayerInputArrows();
	static std::shared_ptr<IPlayerInput> createImmobilePlayerInput();
};

#endif // PLAYERINPUTFACTORY_HPP
