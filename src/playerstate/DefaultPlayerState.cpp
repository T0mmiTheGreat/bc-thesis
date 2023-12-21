/**
 * @file DefaultPlayerState.cpp
 * @author Tomáš Ludrovan
 * @brief DefaultPlayerState class
 * @version 0.1
 * @date 2023-12-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "playerstate/DefaultPlayerState.hpp"

#include <cmath>

#include "playerinput/PlayerInputFactory.hpp"

DefaultPlayerState::DefaultPlayerState(double startX, double startY,
	double startHp, std::shared_ptr<IPlayerInput> playerInput,
	double baseSize, double sizePower, double baseSpeed, double speedPower)
	: PlayerStateBase(startX, startY, startHp, playerInput)
	, baseSize{baseSize}
	, sizePower{sizePower}
	, baseSpeed{baseSpeed}
	, speedPower{speedPower}
{}

DefaultPlayerState::DefaultPlayerState(double startX, double startY,
	std::shared_ptr<IPlayerInput> playerInput)
	: DefaultPlayerState(startX, startY, 1.0, playerInput, DEFAULT_BASE_SIZE,
		DEFAULT_SIZE_POWER, DEFAULT_BASE_SPEED, DEFAULT_SPEED_POWER)
{}

DefaultPlayerState::DefaultPlayerState()
	: DefaultPlayerState(0.0, 0.0, 
		PlayerInputFactory::createImmobilePlayerInput())
{}

double DefaultPlayerState::getSize()
{
	// Full HP -> 35.0
	// 0 HP -> 0.0
	// 2x Full HP -> 70.0 ...? (Or less?)
	// Would probably apply this: `hp^x * 35.0`, where x -> (0, 1)
	return std::pow(hp, sizePower) * 35.0;
	//return hp * 35.0;
}

double DefaultPlayerState::getSpeed()
{
	// Full HP -> 3.0
	// 1/2 full HP -> 6.0 ...?
	// 2x Full HP -> 1.5 ...?
	// `hp^x * 35.0`, where x -> (0, 1)
	return 3.0 / std::pow(hp, speedPower);
	//return 3.0 / hp;
}
