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

void PlayerSprite::repaintCostumeNormal(std::shared_ptr<ICanvas> canvas,
	const Rect& invalidRect)
{
	(void)invalidRect;

	canvas->setFillingColor(m_color);
	canvas->fillCircle(x + m_radius, y + m_radius, m_radius);
}

void PlayerSprite::repaintCostumeHighlighted(std::shared_ptr<ICanvas> canvas,
	const Rect& invalidRect)
{
	repaintCostumeNormal(canvas, invalidRect);

	canvas->setStrokeWidth(1.0);
	canvas->setStrokingColor(HIGHLIGHT_COLOR);
	canvas->strokeCircle(x + m_radius, y + m_radius, m_radius);
}

PlayerSprite::PlayerSprite(std::shared_ptr<IPaintingProxy> paintingProxy)
	: SpriteBase(paintingProxy)
	, BoundedSpriteBase(paintingProxy)
	, PositionedSpriteBase(paintingProxy)
	, m_costume{COSTUME_NORMAL}
	, m_color()
	, m_radius{20}
{}

Size2d PlayerSprite::getSize()
{
	return Size2d(m_radius*2, m_radius*2);
}

void PlayerSprite::repaintAsVisible(std::shared_ptr<ICanvas> canvas, const Rect& invalidRect)
{
	switch (m_costume) {
		case COSTUME_NORMAL:
			repaintCostumeNormal(canvas, invalidRect);
			break;
		case COSTUME_HIGHLIGHTED:
			repaintCostumeHighlighted(canvas, invalidRect);
			break;
	}
}

Color PlayerSprite::getColor() const
{
	return propertyGetter(m_color);
}

void PlayerSprite::setColor(const Color& value)
{
	propertySetterComparable(m_color, value);
}

int PlayerSprite::getRadius() const
{
	return m_radius;
}

void PlayerSprite::setRadius(int value)
{
	if (m_radius != value) {
		invalidate();
		m_radius = value;
		invalidate();
	}
}

PlayerSprite::Costume PlayerSprite::getCostume() const
{
	return m_costume;
}

void PlayerSprite::setCostume(PlayerSprite::Costume value)
{
	if (m_costume != value) {
		invalidate();
		m_costume = value;
		invalidate();
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
