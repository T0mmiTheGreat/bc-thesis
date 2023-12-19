/**
 * @file SpriteBase.cpp
 * @author Tomáš Ludrovan
 * @brief SpriteBase class
 * @version 0.1
 * @date 2023-12-19
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "sprite/SpriteBase.hpp"

#include "sysproxy/SysProxyFactory.hpp"

void SpriteBase::invalidateBounds()
{
	Rect invalidRect(getBounds());
	sysProxy->invalidateRect(invalidRect);
}

SpriteBase::SpriteBase():
	x{0},
	y{0},
	sysProxy{SysProxyFactory::createDefault()}
{}

int SpriteBase::getX()
{
	return this->x;
}

int SpriteBase::getY()
{
	return this->y;
}

void SpriteBase::setX(int value)
{
	setPos(value, getY());
}

void SpriteBase::setY(int value)
{
	setPos(getX(), value);
}

void SpriteBase::setPos(int x, int y)
{
	if (getX() != x || getY() != y) {
		invalidateBounds();
		this->x = x;
		this->y = y;
		invalidateBounds();
	}
}

Rect SpriteBase::getBounds()
{
	return Rect(getX(), getY(), getW(), getH());
}
