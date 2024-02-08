/**
 * @file ObstacleBrushSprite.cpp
 * @author Tomáš Ludrovan
 * @brief ObstacleBrushSprite class
 * @version 0.1
 * @date 2024-02-04
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "sprite/ObstacleBrushSprite.hpp"

void ObstacleBrushSprite::repaintLineColor(std::shared_ptr<ICanvas> canvas,
	const Rect& invalidRect, const Color& color)
{
	(void)invalidRect;

	canvas->setStrokeWidth(1.0);
	canvas->setStrokingColor(color);
	canvas->dashedLine(m_p0.x, m_p0.y, m_p1.x, m_p1.y);
}

void ObstacleBrushSprite::repaintNormal(std::shared_ptr<ICanvas> canvas,
	const Rect& invalidRect)
{
	repaintLineColor(canvas, invalidRect, STROKE_NORMAL);
}

void ObstacleBrushSprite::repaintBad(std::shared_ptr<ICanvas> canvas,
	const Rect& invalidRect)
{
	repaintLineColor(canvas, invalidRect, STROKE_BAD);
}

void ObstacleBrushSprite::repaintDot(std::shared_ptr<ICanvas> canvas,
	const Rect& invalidRect)
{
	(void)invalidRect;

	PointF left   = static_cast<PointF>(m_p0);
	PointF top    = static_cast<PointF>(m_p0);
	PointF right  = static_cast<PointF>(m_p0);
	PointF bottom = static_cast<PointF>(m_p0);

	left.x   -= DOT_RADIUS;
	top.y    -= DOT_RADIUS;
	right.x  += DOT_RADIUS;
	bottom.y += DOT_RADIUS;

	PolygonF pog(left, top, right, bottom);
	
	canvas->setFillingColor(FILL_DOT);
	canvas->fillPolygon(pog);
}

ObstacleBrushSprite::ObstacleBrushSprite(
	std::shared_ptr<IPaintingProxy> paintingProxy)
	: SpriteBase(paintingProxy)
	, BoundedSpriteBase(paintingProxy)
	, m_costume{COSTUME_NORMAL}
{}

Rect ObstacleBrushSprite::getBounds()
{
	switch (m_costume) {
		case COSTUME_NORMAL:
		case COSTUME_BAD: {
			Point topLeft(
				std::min(m_p0.x, m_p1.x),
				std::min(m_p0.y, m_p1.y)
			);
			Size2d size(
				std::abs(m_p1.x - m_p0.x),
				std::abs(m_p1.y - m_p0.y)
			);
			Rect res(topLeft, size);
			return res;
		} break;
		case COSTUME_DOT: {
			Rect res(
				m_p0.x - DOT_RADIUS,
				m_p0.y - DOT_RADIUS,
				DOT_RADIUS * 2,
				DOT_RADIUS * 2
			);
			return res;
		} break;
	}

	// Default
	return Rect();
}

void ObstacleBrushSprite::repaint(std::shared_ptr<ICanvas> canvas,
	const Rect& invalidRect)
{
	switch (m_costume) {
		case COSTUME_NORMAL:
			repaintNormal(canvas, invalidRect);
			break;
		case COSTUME_BAD:
			repaintBad(canvas, invalidRect);
			break;
		case COSTUME_DOT:
			repaintDot(canvas, invalidRect);
			break;
	}
}

ObstacleBrushSprite::Costume ObstacleBrushSprite::getCostume() const
{
	return m_costume;
}

void ObstacleBrushSprite::setCostume(ObstacleBrushSprite::Costume value)
{
	if (m_costume != value) {
		invalidateBounds();
		m_costume = value;
		invalidateBounds();
	}
}

const Point& ObstacleBrushSprite::getP0() const
{
	return m_p0;
}

void ObstacleBrushSprite::setP0(const Point& value)
{
	if (m_p0 != value) {
		invalidateBounds();
		m_p0 = value;
		invalidateBounds();
	}
}

const Point& ObstacleBrushSprite::getP1() const
{
	return m_p1;
}

void ObstacleBrushSprite::setP1(const Point& value)
{
	if (m_p1 != value) {
		invalidateBounds();
		m_p1 = value;
		invalidateBounds();
	}
}
