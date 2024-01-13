/**
 * @file PlayerStateBase.cpp
 * @author Tomáš Ludrovan
 * @brief PlayerStateBase class
 * @version 0.1
 * @date 2023-12-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "playerstate/PlayerStateBase.hpp"

#include <cassert>

PlayerStateBase::PlayerStateBase(double startX, double startY, double startHp,
	std::shared_ptr<IPlayerInput> playerInput)
	: x{startX}
	, y{startY}
	, hp{startHp}
	, playerInput{playerInput}
{
	assert(playerInput != nullptr);
}

double PlayerStateBase::getX()
{
	return x;
}

void PlayerStateBase::setX(double value)
{
	x = value;
}

double PlayerStateBase::getY()
{
	return y;
}

void PlayerStateBase::setY(double value)
{
	y = value;
}

double PlayerStateBase::getHp()
{
	return hp;
}

void PlayerStateBase::setHp(double value)
{
	hp = value;
}

void PlayerStateBase::incX(double value)
{
	incPos(value, 0.0);
}

void PlayerStateBase::incY(double value)
{
	incPos(0.0, value);
}

void PlayerStateBase::incPos(double valueX, double valueY)
{
	x += valueX;
	y += valueY;
}

void PlayerStateBase::incHp(double value)
{
	setHp(getHp() + value);
}

std::shared_ptr<IPlayerInput> PlayerStateBase::getPlayerInputObject()
{
	return playerInput;
}

void PlayerStateBase::setPlayerInputObject(std::shared_ptr<IPlayerInput> value)
{
	assert(value != nullptr);
	playerInput = value;
}
