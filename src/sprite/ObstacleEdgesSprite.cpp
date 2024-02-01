/**
 * @file ObstacleEdgesSprite.cpp
 * @author Tomáš Ludrovan
 * @brief ObstacleEdgesSprite class
 * @version 0.1
 * @date 2024-01-31
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "sprite/ObstacleEdgesSprite.hpp"

Rect ObstacleEdgesSprite::getBounds()
{
	if (m_corners.cornerCount() == 0) {
		// Cannot get bounding box of a polygon which has no corners

		return Rect::createEmpty();
	} else {
		Rect res = static_cast<Rect>(m_corners.getBoundingBox());
		return res;
	}
}

ObstacleEdgesSprite::ObstacleEdgesSprite(
	std::shared_ptr<IPaintingProxy> paintingProxy)
	: SpriteBase(paintingProxy)
	, BoundedSpriteBase(paintingProxy)
{}

void ObstacleEdgesSprite::repaint(std::shared_ptr<ICanvas> canvas,
	Rect& invalidRect)
{
	canvas->setStrokeWidth(1.0);
	canvas->setStrokingColor(STROKE_COLOR);
	for (size_t i = 1; i < m_corners.cornerCount(); i++) {
		const PolygonF::CornerType& p0 = m_corners.corners[i - 1];
		const PolygonF::CornerType& p1 = m_corners.corners[i];
		canvas->strokeLine(p0.x, p0.y, p1.x, p1.y);
	}
}

void ObstacleEdgesSprite::pushCorner(const PolygonF::CornerType& p)
{
	invalidateBounds();
	m_corners.corners.push_back(p);
	invalidateBounds();
}

void ObstacleEdgesSprite::popCorner()
{
	if (m_corners.cornerCount() > 0) {
		invalidateBounds();
		m_corners.corners.pop_back();
		invalidateBounds();
	}
}

const PolygonF::CornerType& ObstacleEdgesSprite::getCorner(int idx) const
{
	return m_corners.corners[idx];
}

void ObstacleEdgesSprite::setCorner(int idx, const PolygonF::CornerType& value)
{
	if (m_corners.corners[idx] != value) {
		invalidateBounds();
		m_corners.corners[idx] = value;
		invalidateBounds();
	}
}

size_t ObstacleEdgesSprite::getCornerCount() const
{
	return m_corners.cornerCount();
}
