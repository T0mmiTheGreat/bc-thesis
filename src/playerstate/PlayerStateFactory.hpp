/**
 * @file PlayerStateFactory.hpp
 * @author Tomáš Ludrovan
 * @brief PlayerStateFactory class
 * @version 0.1
 * @date 2023-12-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef PLAYERSTATEFACTORY_HPP
#define PLAYERSTATEFACTORY_HPP

#include <memory>

#include "playerinput/IPlayerInput.hpp"
#include "playerstate/IPlayerState.hpp"

/**
 * @brief Factory for the IPlayerState interface.
 */
class PlayerStateFactory {
public:
	/**
	 * @brief Constructs a new DefaultPlayerState object.
	 */
	static std::shared_ptr<IPlayerState> createDefault();
	/**
	 * @brief Constructs a new DefaultPlayerState object.
	 * 
	 * @param startX Initial X coordinate.
	 * @param startY Initial Y coordinate.
	 * @param playerInputObject Player input instance.
	 */
	static std::shared_ptr<IPlayerState> createDefault(double startX,
		double startY, std::shared_ptr<IPlayerInput> playerInputObject);
	/**
	 * @brief Constructs a new DefaultPlayerState object.
	 * 
	 * @param startX Starting X position.
	 * @param startY Starting Y position.
	 * @param startHp Starting health points (1.0 is full HP, 2.0 is 2x full
	 *                HP, etc.).
	 * @param playerInputObject Player input instance.
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
	static std::shared_ptr<IPlayerState> createDefault(double startX,
		double startY, double startHp, 
		std::shared_ptr<IPlayerInput> playerInputObject, double baseSize,
		double sizePower, double baseSpeed, double speedPower);
};

#endif // PLAYERSTATEFACTORY_HPP
