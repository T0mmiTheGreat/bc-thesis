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
	Rect res = static_cast<Rect>(m_shape.getBoundingBox());
	return res;
}

ObstacleSprite::ObstacleSprite(std::shared_ptr<IPaintingProxy> paintingProxy,
	const PolygonF& shape)
	: SpriteBase(paintingProxy)
	, BoundedSpriteBase(paintingProxy)
{
	setShape(shape);
}

ObstacleSprite::ObstacleSprite(std::shared_ptr<IPaintingProxy> paintingProxy,
	PolygonF&& shape)
	: SpriteBase(paintingProxy)
	, BoundedSpriteBase(paintingProxy)
{
	setShape(std::move(shape));
}

void ObstacleSprite::repaint(std::shared_ptr<ICanvas> canvas, Rect& invalidRect)
{
	canvas->setFillingColor(Color::grayscale(0x80));
	canvas->fillPolygon(m_shape);

	invalidRect += getBounds();
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

	invalidateBounds();
	m_shape = std::move(value);
	invalidateBounds();
}
