/**
 * @file ImmobilePlayerInput.hpp
 * @author Tomáš Ludrovan
 * @brief ImmobilePlayerInput class
 * @version 0.1
 * @date 2023-12-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef IMMOBILEPLAYERINPUT_HPP
#define IMMOBILEPLAYERINPUT_HPP

#include "playerinput/PlayerInputBase.hpp"

class ImmobilePlayerInput : public PlayerInputBase {
public:
	PlayerInputState readInput() override;
};

#endif // IMMOBILEPLAYERINPUT_HPP
