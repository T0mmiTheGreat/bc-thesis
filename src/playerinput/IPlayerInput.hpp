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

struct PlayerInputState {
	bool left: 1;
	bool up: 1;
	bool right: 1;
	bool down: 1;
};

class IPlayerInput {
public:
	virtual ~IPlayerInput() {}
	virtual PlayerInputState readInput() = 0;
};

#endif // IPLAYERINPUT_HPP
