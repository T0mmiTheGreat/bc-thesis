/**
 * @file PositionedSpriteBase.cpp
 * @author Tomáš Ludrovan
 * @brief PositionedSpriteBase class
 * @version 0.1
 * @date 2024-01-31
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "sprite/PositionedSpriteBase.hpp"

void PositionedSpriteBase::posChangedEvent(int oldX, int oldY, int newX,
	int newY)
{
	Size2d spriteSize = getSize();
	paintingProxy->invalidateRect(Rect(oldX, oldY, spriteSize.w, spriteSize.h));
	paintingProxy->invalidateRect(Rect(newX, newY, spriteSize.w, spriteSize.h));
}

PositionedSpriteBase::PositionedSpriteBase(
	std::shared_ptr<IPaintingProxy> paintingProxy)
	: BoundedSpriteBase(paintingProxy)
	, x{0}
	, y{0}
{}

int PositionedSpriteBase::getX()
{
	return x;
}

int PositionedSpriteBase::getY()
{
	return y;
}

int PositionedSpriteBase::getW()
{
	return getSize().w;
}

int PositionedSpriteBase::getH()
{
	return getSize().h;
}

void PositionedSpriteBase::setX(int value)
{
	setPos(value, this->y);
}

void PositionedSpriteBase::setY(int value)
{
	setPos(this->x, value);
}

void PositionedSpriteBase::setPos(int x, int y)
{
	if (getX() != x || getY() != y) {
		// Remember old values
		int oldX = this->x;
		int oldY = this->y;
		// Set new values
		this->x = x;
		this->y = y;
		// Notify
		posChangedEvent(oldX, oldY, x, y);
	}
}

void PositionedSpriteBase::setPos(Point p)
{
	setPos(p.x, p.y);
}

Rect PositionedSpriteBase::getBounds()
{
	Size2d size = getSize();
	Rect res(x, y, size);
	return res;
}
