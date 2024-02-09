/**
 * @file ObstacleSprite.cpp
 * @author Tomáš Ludrovan
 * @brief ObstacleSprite class
 * @version 0.1
 * @date 2024-01-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "sprite/ObstacleSprite.hpp"

Rect ObstacleSprite::getBounds()
{
	if (m_shape.cornerCount() == 0) {
		return Rect::createEmpty();
	} else {
		Rect res = static_cast<Rect>(m_shape.getBoundingBox());
		return res;
	}
}

void ObstacleSprite::repaintCostumeNormal(std::shared_ptr<ICanvas> canvas,
	const Rect& invalidRect)
{
	(void)invalidRect;

	if (m_shape.cornerCount() != 0) {
		canvas->setFillingColor(m_color);
		canvas->fillPolygon(m_shape);
	}
}

void ObstacleSprite::repaintCostumeHighlighted(std::shared_ptr<ICanvas> canvas,
	const Rect& invalidRect)
{
	repaintCostumeNormal(canvas, invalidRect);

	if (m_shape.cornerCount() != 0) {
		canvas->setStrokeWidth(1.0);
		canvas->setStrokingColor(HIGHLIGHT_COLOR);
		canvas->strokePolygon(m_shape);
	}
}

ObstacleSprite::ObstacleSprite(std::shared_ptr<IPaintingProxy> paintingProxy)
	: SpriteBase(paintingProxy)
	, BoundedSpriteBase(paintingProxy)
	, m_costume{COSTUME_NORMAL}
{}

void ObstacleSprite::repaintAsVisible(std::shared_ptr<ICanvas> canvas, const Rect& invalidRect)
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

Color ObstacleSprite::getColor() const
{
	return m_color;
}

void ObstacleSprite::setColor(const Color& value)
{
	if (m_color != value) {
		invalidate();
		m_color = value;
		invalidate();
	}
}

const PolygonF& ObstacleSprite::getShape() const
{
	return m_shape;
}

void ObstacleSprite::setShape(const PolygonF& value)
{
	PolygonF v = value;
	setShape(std::move(v));
}

void ObstacleSprite::setShape(PolygonF&& value)
{
	assert(value.isValidEuclidean());

	invalidate();
	m_shape = std::move(value);
	invalidate();
}

ObstacleSprite::Costume ObstacleSprite::getCostume() const
{
	return m_costume;
}

void ObstacleSprite::setCostume(ObstacleSprite::Costume value)
{
	if (m_costume != value) {
		invalidate();
		m_costume = value;
		invalidate();
	}
}
