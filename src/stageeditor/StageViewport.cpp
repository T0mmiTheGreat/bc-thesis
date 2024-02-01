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
	if (m_srcRect.getRight() < 0) {
		// Too much on the left; stick to the right side of the workspace area
		m_srcRect.offset(-m_srcRect.getRight(), 0.0);
	} else if (m_srcRect.x > m_stageSize.w) {
		// Too much on the right; stick to the left side of the workspace area
		m_srcRect.x = m_stageSize.w;
	}

	if (m_srcRect.getBottom() < 0) {
		// Too low; stick to the bottom side of the workspace area
		m_srcRect.offset(0.0, -m_srcRect.getBottom());
	} else if (m_srcRect.y > m_stageSize.h) {
		// Too high; stick to the top side of the workspace area
		m_srcRect.y = m_stageSize.h;
	}
}

StageViewport::StageViewport(const Size2dF& stageSize, const Size2dF& dstSize)
	: m_srcRect(0.0, 0.0, dstSize)
	, m_dstSize{dstSize}
	, m_isDragging{false}
	, m_zoom{1.0}
{
	setStageSize(stageSize);
}

void StageViewport::setStagePos(PointF::ValueType x, PointF::ValueType y)
{
	// We consider the stage position to be always [0,0], so instead of moving
	// the stage, we basically just move the "camera".
	// If we want to move the stage to the right, we need to move the camera to
	// the left. If we want to move the stage up, we must move the camera down.

	m_srcRect.x = -x;
	m_srcRect.y = -y;
}

void StageViewport::setStagePos(const PointF& pos)
{
	setStagePos(pos.x, pos.y);
}

void StageViewport::stageCenter()
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

PointF StageViewport::screenToStage(const PointF& p)
{
	PointF res = p;

	// Remove zoom
	res.x /= m_zoom;
	res.y /= m_zoom;

	// Relative to src rect
	res.x += m_srcRect.x;
	res.y += m_srcRect.y;

	return res;
}

RectF StageViewport::screenToStage(const RectF& r)
{
	RectF res(
		screenToStage(r.getTopLeft()),
		screenToStage(r.getBottomRight())
	);
	return res;
}

PointF StageViewport::stageToScreen(const PointF& p)
{
	// Relative to [0, 0] of the stage
	PointF res = p.relativeTo(m_srcRect.getTopLeft());

	// Apply zoom
	res.x *= m_zoom;
	res.y *= m_zoom;

	return res;
}

RectF StageViewport::stageToScreen(const RectF& r)
{
	RectF res(
		stageToScreen(r.getTopLeft()),
		stageToScreen(r.getBottomRight())
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
		// Vector `dragBegin -> where`
		PointF dragRelToBegin = where.relativeTo(m_dragBegin);
		// Project the vector to stage space
		PointF dragMinusZoom(
			dragRelToBegin.x / m_zoom,
			dragRelToBegin.y / m_zoom
		);
		// Apply the vector to the srcRectBegin to get the new position of
		// src rect
		m_srcRect.setPos(m_srcRectBegin.relativeTo(dragMinusZoom));
		pullLeash();
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
	RectF offsetRect(towards, PointF::zero());
	// After this operation the X and Y position of the rectangle will be the
	// `towards` point projected to the stage space. By changing the size of
	// this rectangle afterwrds, we are finding the new position for the src
	// rect -- the `bottomRight` property of `offsetRect` will be our new
	// position (topLeft) of the src rect.
	offsetRect = screenToStage(offsetRect);
	offsetRect.w *= zoomRatio;
	offsetRect.h *= zoomRatio;

	// Update the src rect
	m_srcRect.setPos(offsetRect.getBottomRight());
	m_srcRect.w *= zoomRatio;
	m_srcRect.h *= zoomRatio;


	// Update the zoom
	m_zoom = newZoom;
	pullLeash();
}

void StageViewport::setZoom(ZoomType newZoom)
{
	setZoom(PointF(m_dstSize.w/2, m_dstSize.h/2), newZoom);
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
	// First translate ("move it to the grid"), then scale ("apply zoom")

	// |  1  0  0 |   | sx  0  0 |   |    sx     0     0 |
	// |  0  1  0 | x |  0 sy  0 | = |     0    sy     0 |
	// | tx ty  1 |   |  0  0  1 |   | sx*tx sy*ty     1 |
	Matrix3x3 res(
		               m_zoom,                     0,                     0,
		                    0,                m_zoom,                     0,
		-m_srcRect.x * m_zoom, -m_srcRect.y * m_zoom,                     1
	);
	return res;
}

Matrix3x3 StageViewport::getProjectionToStageMatrix() const
{
	// First scale ("remove zoom"), then translate ("move from grid to stage")

	// | sx  0  0 |   |  1  0  0 |   | sx  0  0 |
	// |  0 sy  0 | x |  0  1  0 | = |  0 sy  0 |
	// |  0  0  1 |   | tx ty  1 |   | tx ty  1 |
	double s = 1/m_zoom;
	Matrix3x3 res(
		          s,           0,           0,
				  0,           s,           0,
		m_srcRect.x, m_srcRect.y,           1
	);
	return res;
}
