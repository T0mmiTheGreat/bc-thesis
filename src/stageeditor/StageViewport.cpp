/**
 * @file StageViewport.cpp
 * @author Tomáš Ludrovan
 * @brief StageViewport class
 * @version 0.1
 * @date 2024-01-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "stageeditor/StageViewport.hpp"

void StageViewport::pullLeash()
{
}

StageViewport::StageViewport(const Size2dF& stageSize, const RectF& dstRect)
	: m_srcRect(0.0, 0.0, dstRect.getSize())
	, m_dstRect{dstRect}
	, m_isDragging{false}
	, m_zoom{1.0}
{
	setStageSize(stageSize);
}

void StageViewport::srcRectSetLocation(PointF::ValueType x, PointF::ValueType y)
{
	m_srcRect.x = x;
	m_srcRect.y = y;
}

void StageViewport::srcRectSetLocation(const PointF& location)
{
	srcRectSetLocation(location.x, location.y);
}

void StageViewport::srcRectCenter()
{
	m_srcRect.centerAt(RectF(0.0, 0.0, m_stageSize));
}

void StageViewport::setStageSize(Size2dF::ValueType w, Size2dF::ValueType h)
{
	setStageSize(Size2dF(w, h));
}

void StageViewport::setStageSize(const Size2dF& size)
{
	m_stageSize = size;
	pullLeash();
}

PointF StageViewport::dstToSrc(const PointF& p)
{
	// Relative to [0, 0] of the workspace
	PointF res = p.relativeTo(m_dstRect.getTopLeft());

	// Remove zoom
	res.x /= m_zoom;
	res.y /= m_zoom;

	// Relative to src rect
	res.x += m_srcRect.x;
	res.y += m_srcRect.y;

	return res;
}

RectF StageViewport::dstToSrc(const RectF& r)
{
	RectF res(
		dstToSrc(r.getTopLeft()),
		dstToSrc(r.getBottomRight())
	);
	return res;
}

PointF StageViewport::srcToDst(const PointF& p)
{
	// Relative to [0, 0] of the stage
	PointF res = p.relativeTo(m_srcRect.getTopLeft());

	// Apply zoom
	res.x *= m_zoom;
	res.y *= m_zoom;

	// Relative to dst rect
	res.x += m_dstRect.x;
	res.y += m_dstRect.y;

	return res;
}

RectF StageViewport::srcToDst(const RectF& r)
{
	RectF res(
		srcToDst(r.getTopLeft()),
		srcToDst(r.getBottomRight())
	);
	return res;
}

void StageViewport::beginDrag(const PointF& where)
{
	m_dragBegin = where;
	m_srcRectBegin = m_srcRect.getTopLeft();
	m_isDragging = true;
}

void StageViewport::doDrag(const PointF& where)
{
	if (m_isDragging) {
		PointF dragRelToBegin = where.relativeTo(m_dragBegin);
		PointF dragMinusZoom(
			dragRelToBegin.x / m_zoom,
			dragRelToBegin.y / m_zoom
		);
		m_srcRect.setPos(m_srcRectBegin.relativeTo(dragMinusZoom));
	}
}

void StageViewport::endDrag()
{
	m_isDragging = false;
}

bool StageViewport::isDrag()
{
	return m_isDragging;
}

void StageViewport::setZoom(const PointF& towards, ZoomType newZoom)
{
	assert(newZoom != 0.0);

	ZoomType oldZoom = m_zoom;
	// The ratio of oldZoom:newZoom
	ZoomType zoomRatio = oldZoom / newZoom;


	// Updating the src rect

	// Notice that the rectangle is not normalized, i.e., has negative dimensions.
	RectF offsetRect(towards, m_dstRect.getTopLeft());
	// After this operation the X and Y position of the rectangle will be the `towards`
	// point projected to the "stage". By changing the size of this rectangle afterwrds,
	// we are finding the new position for the src rect -- the `bottomRight` property
	// of `offsetRect` will be our new position (topLeft) of the src rect.
	offsetRect = dstToSrc(offsetRect);
	offsetRect.w *= zoomRatio;
	offsetRect.h *= zoomRatio;

	// Update the src rect
	m_srcRect.setPos(offsetRect.getBottomRight());
	m_srcRect.w *= zoomRatio;
	m_srcRect.h *= zoomRatio;


	// Update the zoom
	m_zoom = newZoom;
}

void StageViewport::setZoom(ZoomType newZoom)
{
	setZoom(m_dstRect.getCenterPoint(), newZoom);
}

void StageViewport::zoomIn(const PointF& towards, ZoomType factor)
{
	setZoom(towards, m_zoom * factor);
}

void StageViewport::zoomIn(ZoomType factor)
{
	setZoom(m_zoom * factor);
}

void StageViewport::zoomOut(const PointF& towards, ZoomType factor)
{
	setZoom(towards, m_zoom / factor);
}

void StageViewport::zoomOut(ZoomType factor)
{
	setZoom(m_zoom / factor);
}

RectF StageViewport::getSrcRect() const
{
	return m_srcRect;
}

StageViewport::ZoomType StageViewport::getZoom() const
{
	return m_zoom;
}

Matrix3x3 StageViewport::getProjectionToScreenMatrix() const
{
	// | sx  0  0 |   |  1  0  0 |   | sx  0  0 |
	// |  0 sy  0 | x |  0  1  0 | = |  0 sy  0 |
	// |  0  0  1 |   | tx ty  1 |   | tx ty  1 |
	return Matrix3x3(
              m_zoom,            0,            0,
			       0,       m_zoom,            0,
		-m_srcRect.x, -m_srcRect.y,            1
	);
}

Matrix3x3 StageViewport::getProjectionToWorkspaceMatrix() const
{
	return Matrix3x3(
           1/m_zoom,           0,           0,
		          0,    1/m_zoom,           0,
		m_srcRect.x, m_srcRect.y,           1
	);
}
