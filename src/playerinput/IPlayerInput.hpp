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

/**
 * @brief Structure of flags representing the action requested by player.
 */
struct PlayerInputState {
	bool left: 1;
	bool up: 1;
	bool right: 1;
	bool down: 1;

	PlayerInputState()
		: left{false}
		, up{false}
		, right{false}
		, down{false}
	{}
};

/**
 * @brief Interface for classes which send player input to the game core.
 */
class IPlayerInput {
public:
	virtual ~IPlayerInput() {}
	/**
	 * @brief Obtains the actions requested by the player.
	 */
	virtual PlayerInputState readInput() = 0;
};

#endif // IPLAYERINPUT_HPP
