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

/**
 * @brief Player input which does not read any input.
 * 
 * @details There are surely situations where this class finds its use.
 */
class ImmobilePlayerInput : public PlayerInputBase {
public:
	PlayerInputFlags readInput() override;
};

#endif // IMMOBILEPLAYERINPUT_HPP
