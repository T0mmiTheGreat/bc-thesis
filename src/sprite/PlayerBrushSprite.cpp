/**
 * @file PlayerBrushSprite.cpp
 * @author Tomáš Ludrovan
 * @brief PlayerBrushSprite class
 * @version 0.1
 * @date 2024-02-03
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "sprite/PlayerBrushSprite.hpp"

void PlayerBrushSprite::repaintColor(std::shared_ptr<ICanvas> canvas,
	Rect& invalidRect, const Color& color)
{
	canvas->setStrokeWidth(1.0);
	canvas->setStrokingColor(color);
	canvas->dashedCircle(getCenterX(), getCenterY(), getRadius());

	invalidRect += getBounds();
}

void PlayerBrushSprite::repaintNormal(std::shared_ptr<ICanvas> canvas,
	Rect& invalidRect)
{
	repaintColor(canvas, invalidRect, STROKE_NORMAL);
}

void PlayerBrushSprite::repaintBad(std::shared_ptr<ICanvas> canvas,
	Rect& invalidRect)
{
	repaintColor(canvas, invalidRect, STROKE_BAD);
}

PlayerBrushSprite::PlayerBrushSprite(
	std::shared_ptr<IPaintingProxy> paintingProxy)
	: SpriteBase(paintingProxy)
	, BoundedSpriteBase(paintingProxy)
	, PositionedSpriteBase(paintingProxy)
	, m_costume{COSTUME_NORMAL}
{}

Size2d PlayerBrushSprite::getSize()
{
	Size2d res(2*m_radius, 2*m_radius);
	return res;
}

void PlayerBrushSprite::repaint(std::shared_ptr<ICanvas> canvas,
	Rect& invalidRect)
{
	switch (m_costume) {
		case COSTUME_NORMAL:
			repaintNormal(canvas, invalidRect);
			break;
		case COSTUME_BAD:
			repaintBad(canvas, invalidRect);
			break;
	}
}

PlayerBrushSprite::Costume PlayerBrushSprite::getCostume() const
{
	return m_costume;
}

void PlayerBrushSprite::setCostume(PlayerBrushSprite::Costume value)
{
	if (m_costume != value) {
		invalidateBounds();
		m_costume = value;
		invalidateBounds();
	}
}

int PlayerBrushSprite::getRadius() const
{
	return m_radius;
}

void PlayerBrushSprite::setRadius(int value)
{
	if (m_radius != value) {
		invalidateBounds();
		m_radius = value;
		invalidateBounds();
	}
}

int PlayerBrushSprite::getCenterX() const
{
	return x + m_radius;
}

int PlayerBrushSprite::getCenterY() const
{
	return y + m_radius;
}

Point PlayerBrushSprite::getCenterPos() const
{
	Point res(x + m_radius, y + m_radius);
	return res;
}

void PlayerBrushSprite::setCenterX(int value)
{
	setX(value - m_radius);
}

void PlayerBrushSprite::setCenterY(int value)
{
	setY(value - m_radius);
}

void PlayerBrushSprite::setCenterPos(int x, int y)
{
	setPos(x - m_radius, y - m_radius);
}

void PlayerBrushSprite::setCenterPos(const Point& pos)
{
	Point p = pos;
	p.x -= getRadius();
	p.y -= getRadius();
	setPos(p);
}
