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
#include "playerinput/ISysProxyplayerInput.hpp"

/**
 * @brief Factory for the IPlayerInput interface.
 */
class PlayerInputFactory {
public:
	/**
	 * @brief Constructs a new PlayerInputFactory object.
	 * 
	 * @param keyLeft Key code for the "move left" action.
	 * @param keyUp Key code for the "move up" action.
	 * @param keyRight Key code for the "move right" action.
	 * @param keyDown Key code for the "move down" action.
	 */
	static std::shared_ptr<IPlayerInput> createKeyboardPlayerInput(
		std::shared_ptr<ISysProxyPlayerInput> sysProxy, KeyCode keyLeft,
		KeyCode keyUp, KeyCode keyRight, KeyCode keyDown);
	static std::shared_ptr<IPlayerInput> createKeyboardPlayerInputWSAD(
		std::shared_ptr<ISysProxyPlayerInput> sysProxy);
	static std::shared_ptr<IPlayerInput> createKeyboardPlayerInputArrows(
		std::shared_ptr<ISysProxyPlayerInput> sysProxy);
	static std::shared_ptr<IPlayerInput> createImmobilePlayerInput();
};

#endif // PLAYERINPUTFACTORY_HPP
