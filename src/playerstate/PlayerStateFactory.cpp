/**
 * @file PlayerStateFactory.cpp
 * @author Tomáš Ludrovan
 * @brief PlayerStateFactory class
 * @version 0.1
 * @date 2023-12-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "playerstate/PlayerStateFactory.hpp"

#include "playerstate/DefaultPlayerState.hpp"

std::shared_ptr<IPlayerState> PlayerStateFactory::createDefault()
{
	return std::make_shared<DefaultPlayerState>();
}

std::shared_ptr<IPlayerState> PlayerStateFactory::createDefault(double startX,
	double startY, std::shared_ptr<IPlayerInput> playerInputObject)
{
	return std::make_shared<DefaultPlayerState>(startX, startY, playerInputObject);
}

std::shared_ptr<IPlayerState> PlayerStateFactory::createDefault(double startX,
	double startY, double startHp,
	std::shared_ptr<IPlayerInput> playerInputObject, double baseSize,
	double sizePower, double baseSpeed, double speedPower)
{
	return std::make_shared<DefaultPlayerState>(startX, startY, startHp,
		playerInputObject, baseSize, sizePower, baseSpeed, speedPower);
}
