/**
 * @file PlayerInputBase.hpp
 * @author Tomáš Ludrovan
 * @brief PlayerInputBase class
 * @version 0.1
 * @date 2023-12-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef PLAYERINPUTBASE_HPP
#define PLAYERINPUTBASE_HPP

#include "playerinput/IPlayerInput.hpp"

/**
 * @brief Base class for all player inputs.
 * 
 * @details In future, there might be some definitions common to all player,
 *          inputs so it is better for all inputs to inherit from this one.
 */
class PlayerInputBase : public IPlayerInput {};

#endif // PLAYERINPUTBASE_HPP
