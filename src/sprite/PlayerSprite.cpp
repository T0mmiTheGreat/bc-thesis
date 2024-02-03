/**
 * @file PlayerSprite.cpp
 * @author Tomáš Ludrovan
 * @brief PlayerSprite class
 * @version 0.1
 * @date 2024-01-13
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "sprite/PlayerSprite.hpp"

PlayerSprite::PlayerSprite(std::shared_ptr<IPaintingProxy> paintingProxy)
	: SpriteBase(paintingProxy)
	, BoundedSpriteBase(paintingProxy)
	, PositionedSpriteBase(paintingProxy)
	, m_color()
	, m_radius{20}
{}

Size2d PlayerSprite::getSize()
{
	return Size2d(m_radius*2, m_radius*2);
}

void PlayerSprite::repaint(std::shared_ptr<ICanvas> canvas, Rect& invalidRect)
{
	canvas->setFillingColor(m_color);
	canvas->fillCircle(x + m_radius, y + m_radius, m_radius);
}

Color PlayerSprite::getColor() const
{
	return m_color;
}

void PlayerSprite::setColor(const Color& value)
{
	if (m_color != value) {
		m_color = value;
		invalidateBounds();
	}
}

int PlayerSprite::getRadius() const
{
	return m_radius;
}

void PlayerSprite::setRadius(int value)
{
	if (m_radius != value) {
		invalidateBounds();
		m_radius = value;
		invalidateBounds();
	}
}

void PlayerSprite::setCenterX(int value)
{
	setX(value - getRadius());
}

void PlayerSprite::setCenterY(int value)
{
	setY(value - getRadius());
}

void PlayerSprite::setCenterPos(int x, int y)
{
	setPos(x - getRadius(), y - getRadius());
}

void PlayerSprite::setCenterPos(const Point& pos)
{
	Point p = pos;
	p.x -= getRadius();
	p.y -= getRadius();
	setPos(p);
}
