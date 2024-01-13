/**
 * @file IPlayerState.hpp
 * @author Tomáš Ludrovan
 * @brief IPlayerState interface
 * @version 0.1
 * @date 2023-12-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef IPLAYERSTATE_HPP
#define IPLAYERSTATE_HPP

#include <memory>

#include "playerinput/IPlayerInput.hpp"

/**
 * @brief Interface for the player-related information (position, HP, etc).
 * 
 * @details Not sure whether there are going to be more than one implementations
 *          of this interface, but there's nothing wrong with having interface
 *          for merely one implementation.
 */
class IPlayerState {
public:
	virtual ~IPlayerState() {}
	/**
	 * @brief Getter for the X coordinate.
	 */
	virtual double getX() = 0;
	/**
	 * @brief Setter for the X coordinate.
	 */
	virtual void setX(double value) = 0;
	/**
	 * @brief Getter for the Y coordinate.
	 */
	virtual double getY() = 0;
	/**
	 * @brief Setter for the Y coordinate.
	 */
	virtual void setY(double value) = 0;
	/**
	 * @brief Getter for the player health points value.
	 */
	virtual double getHp() = 0;
	/**
	 * @brief Setter for the player health points value.
	 */
	virtual void setHp(double value) = 0;
	/**
	 * @brief Getter for the player size (diameter).
	 * 
	 * @details Derived from the player HP.
	 */
	virtual double getSize() = 0;
	/**
	 * @brief Getter for the player movement speed.
	 * 
	 * @details Derived from the player HP.
	 */
	virtual double getSpeed() = 0;
	/**
	 * @brief Increments the player's X coordinate by `value`.
	 */
	virtual void incX(double value) = 0;
	/**
	 * @brief Increments the player's Y coordinate by `value`.
	 */
	virtual void incY(double value) = 0;
	/**
	 * @brief Increments the player's X and Y coordinates.
	 */
	virtual void incPos(double valueX, double valueY) = 0;
	/**
	 * @brief Increments the player's HP by `value`.
	 */
	virtual void incHp(double value) = 0;
	/**
	 * @brief Getter for the player input object.
	 * 
	 * @return The assigned input object or nullptr if none assigned.
	 */
	virtual std::shared_ptr<IPlayerInput> getPlayerInputObject() = 0;
	/**
	 * @brief Setter for the player input object.
	 */
	virtual void setPlayerInputObject(std::shared_ptr<IPlayerInput> value) = 0;
};

#endif // IPLAYERSTATE_HPP
