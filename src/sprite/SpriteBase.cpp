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
	sysProxy->invalidateRect(getBounds());
}

void SpriteBase::posChangedEvent(int oldX, int oldY, int newX, int newY)
{
	Size2d spriteSize = getSize();
	sysProxy->invalidateRect(Rect(oldX, oldY, spriteSize.w, spriteSize.h));
	sysProxy->invalidateRect(Rect(newX, newY, spriteSize.w, spriteSize.h));
}

SpriteBase::SpriteBase()
	: x{0}
	, y{0}
	, isAnimationRunningFlag{false}
	, sysProxy{SysProxyFactory::createDefault()}
{}

int SpriteBase::getX()
{
	return this->x;
}

int SpriteBase::getY()
{
	return this->y;
}

int SpriteBase::getW()
{
	return getSize().w;
}

int SpriteBase::getH()
{
	return getSize().h;
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
		int oldX = this->x;
		int oldY = this->y;
		this->x = x;
		this->y = y;
		posChangedEvent(oldX, oldY, x, y);
	}
}

Rect SpriteBase::getBounds()
{
	Size2d size = getSize();
	return Rect(getX(), getY(), size.w, size.h);
}

void SpriteBase::startAnimation()
{
	if (isAnimationRunning()) {
		stopAnimation();
	}
	isAnimationRunningFlag = true;
}

void SpriteBase::stopAnimation()
{
	isAnimationRunningFlag = false;
}

bool SpriteBase::isAnimationRunning()
{
	return isAnimationRunningFlag;
}

void SpriteBase::frameEvent()
{
}
