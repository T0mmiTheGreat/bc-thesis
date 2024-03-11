/**
 * @file BonusSprite.cpp
 * @author Tomáš Ludrovan
 * @brief BonusSprite class
 * @version 0.1
 * @date 2024-03-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "sprite/BonusSprite.hpp"

void BonusSprite::repaintAsVisible(std::shared_ptr<ICanvas> canvas,
	const Rect& invalidRect)
{
	(void)invalidRect;

	Rect dstRect(getX(), getY(), 2*getRadius(), 2*getRadius());
	canvas->copyImage(IMAGE_ID, dstRect);
}

BonusSprite::BonusSprite(std::shared_ptr<IPaintingProxy> paintingProxy)
	: SpriteBase(paintingProxy)
	, BoundedSpriteBase(paintingProxy)
	, PositionedSpriteBase(paintingProxy)
	, m_radius{20} // Just put there some non-zero value...
{}

Size2d BonusSprite::getSize()
{
	return Size2d(m_radius*2, m_radius*2);
}

int BonusSprite::getRadius() const
{
	return propertyGetter(m_radius);
}

void BonusSprite::setRadius(int value)
{
	propertySetterComparable(m_radius, value);
}

void BonusSprite::setCenterX(int value)
{
	setX(value - getRadius());
}

void BonusSprite::setCenterY(int value)
{
	setY(value - getRadius());
}

void BonusSprite::setCenterPos(int x, int y)
{
	setPos(x - getRadius(), y - getRadius());
}

void BonusSprite::setCenterPos(const Point & pos)
{
	Point p = pos;
	p.x -= getRadius();
	p.y -= getRadius();
	setPos(p);
}
