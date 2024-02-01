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
	if (m_corners.empty()) {
		// No corners => empty boundary

		return Rect::createEmpty();
	} else {
		// The corners actually form a polygon which is not closed. We can take
		// advantage of this, and create a PolygonF using the corners, then get
		// the bounding box of that polygon.

		PolygonF pog(std::move(m_corners));
		Rect res = static_cast<Rect>(pog.getBoundingBox());
		m_corners = std::move(pog.corners);
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
	if (!m_corners.empty()) {
		canvas->setStrokeWidth(1.0);
		canvas->setStrokingColor(STROKE_COLOR);
		for (size_t i = 1; i < m_corners.size(); i++) {
			const PolygonF::CornerType& p0 = m_corners[i - 1];
			const PolygonF::CornerType& p1 = m_corners[i];
			canvas->strokeLine(p0.x, p0.y, p1.x, p1.y);
		}
	}
}

void ObstacleEdgesSprite::setCorners(const std::vector<PointF>& value)
{
	std::vector<PointF> v = value;
	setCorners(std::move(v));
}

void ObstacleEdgesSprite::setCorners(std::vector<PointF>&& value)
{
	invalidateBounds();
	m_corners = std::move(value);
	// If its empty then there's nothing to invalidate
	if (!m_corners.empty()) {
		invalidateBounds();
	}
}

void ObstacleEdgesSprite::clearCorners()
{
	invalidateBounds();
	m_corners.clear();
}
