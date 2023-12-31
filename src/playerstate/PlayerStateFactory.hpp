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

class PlayerStateFactory {
public:
	static std::shared_ptr<IPlayerState> createDefault();
	static std::shared_ptr<IPlayerState> createDefault(double startX,
		double startY, std::shared_ptr<IPlayerInput> playerInputObject);
	static std::shared_ptr<IPlayerState> createDefault(double startX,
		double startY, double startHp, 
		std::shared_ptr<IPlayerInput> playerInputObject, double baseSize,
		double sizePower, double baseSpeed, double speedPower);
};

#endif // PLAYERSTATEFACTORY_HPP
