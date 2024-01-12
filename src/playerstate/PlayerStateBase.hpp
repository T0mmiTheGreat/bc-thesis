/**
 * @file PlayerStateBase.hpp
 * @author Tomáš Ludrovan
 * @brief PlayerStateBase class
 * @version 0.1
 * @date 2023-12-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef PLAYERSTATEBASE_HPP
#define PLAYERSTATEBASE_HPP

#include <memory>

#include "playerstate/IPlayerState.hpp"

/**
 * @brief Base class for player state implementations.
 */
class PlayerStateBase : public IPlayerState {
protected:
	/**
	 * @brief Value to multiply with the player HP value when displaying it.
	 */
	const double HP_FACTOR = 100.0;

	double x;
	double y;
	double hp;
	std::shared_ptr<IPlayerInput> playerInput;
public:
	/**
	 * @brief Constructs a new PlayerStateBase object.
	 * 
	 * @param startX Initial X coordinate.
	 * @param startY Initial Y coordinate.
	 * @param startHp Initial health points.
	 * @param playerInput Player input instance.
	 */
	PlayerStateBase(double startX, double startY, double startHp,
		std::shared_ptr<IPlayerInput> playerInput);
	/**
	 * @brief Getter for the X coordinate.
	 */
	virtual double getX() override;
	/**
	 * @brief Setter for the X coordinate.
	 */
	virtual void setX(double value) override;
	/**
	 * @brief Getter for the Y coordinate.
	 */
	virtual double getY() override;
	/**
	 * @brief Setter for the Y coordinate.
	 */
	virtual void setY(double value) override;
	/**
	 * @brief Getter for the player health points value.
	 */
	virtual double getHp() override;
	/**
	 * @brief Setter for the player health points value.
	 */
	virtual void setHp(double value) override;
	/**
	 * @brief Increments the player's X coordinate by `value`.
	 */
	virtual void deltaX(double value) override;
	/**
	 * @brief Increments the player's Y coordinate by `value`.
	 */
	virtual void deltaY(double value) override;
	/**
	 * @brief Increments the player's X and Y coordinates.
	 */
	virtual void deltaPos(double valueX, double valueY) override;
	/**
	 * @brief Increments the player's HP by `value`.
	 */
	virtual void deltaHp(double value) override;
	/**
	 * @brief Getter for the player input object.
	 * 
	 * @return The assigned input object or nullptr if none assigned.
	 */
	virtual std::shared_ptr<IPlayerInput> getPlayerInputObject() override;
	/**
	 * @brief Setter for the player input object.
	 */
	virtual void setPlayerInputObject(std::shared_ptr<IPlayerInput> value) override;
};

#endif // PLAYERSTATEBASE_HPP
