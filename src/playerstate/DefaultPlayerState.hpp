/**
 * @file DefaultPlayerState.hpp
 * @author Tomáš Ludrovan
 * @brief DefaultPlayerState class
 * @version 0.1
 * @date 2023-12-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef DEFAULTPLAYERSTATE_HPP
#define DEFAULTPLAYERSTATE_HPP

#include "playerstate/PlayerStateBase.hpp"

class DefaultPlayerState : public PlayerStateBase {
public:
	// Default values, i.e., not provided in the constructor

	static constexpr double DEFAULT_BASE_SIZE = 32.0;
	static constexpr double DEFAULT_SIZE_POWER = 1.0;
	static constexpr double DEFAULT_BASE_SPEED = 3.0;
	static constexpr double DEFAULT_SPEED_POWER = 1.0;

	const double baseSize;
	const double sizePower;
	const double baseSpeed;
	const double speedPower;
public:
	DefaultPlayerState();
	/**
	 * @brief Constructs a new DefaultPlayerState object.
	 * 
	 * @param startX Initial X coordinate.
	 * @param startY Initial Y coordinate.
	 * @param playerInput Player input instance.
	 */
	DefaultPlayerState(double startX, double startY,
		std::shared_ptr<IPlayerInput> playerInput);
	/**
	 * @brief Constructs a new DefaultPlayerState object.
	 * 
	 * @param startX Starting X position.
	 * @param startY Starting Y position.
	 * @param startHp Starting health points (1.0 is full HP, 2.0 is 2x full
	 *                HP, etc.).
	 * @param playerInput Player input instance.
	 * @param baseSize Player sprite radius at full HP.
	 * @param sizePower Power to apply to current HP in size calculations.
	 *                  Interval (0,1) causes the player size to change faster
	 *                  on lower HP (< 1) and slower on higher HP. Interval
	 *                  (1, INF) causes the opposite.
	 * @param baseSpeed Player movement speed at full HP.
	 * @param speedPower Power to apply to current HP in speed calculations.
	 *                   Interval (0,1) causes the player speed to change faster
	 *                   on lower HP (< 1) and slower on higher HP. Interval
	 *                   (1, INF) causes the opposite.
	 */
	DefaultPlayerState(double startX, double startY, double startHp,
		std::shared_ptr<IPlayerInput> playerInput, double baseSize,
		double sizePower, double baseSpeed, double speedPower);
	
	double getSize() override;
	double getSpeed() override;
};

#endif // DEFAULTPLAYERSTATE_HPP
