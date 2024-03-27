/**
 * @file IPlayerInput.hpp
 * @author Tomáš Ludrovan
 * @brief IPlayerInput interface
 * @version 0.1
 * @date 2023-12-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef IPLAYERINPUT_HPP
#define IPLAYERINPUT_HPP

#include "playerinput/PlayerInputFlags.hpp"

/**
 * @brief Interface for classes which send player input to the game core.
 */
class IPlayerInput {
public:
	virtual ~IPlayerInput() {}
	/**
	 * @brief Obtains the actions requested by the player.
	 */
	virtual PlayerInputFlags readInput() = 0;
};

#endif // IPLAYERINPUT_HPP
