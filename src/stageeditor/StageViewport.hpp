/**
 * @file StageViewport.hpp
 * @author Tomáš Ludrovan
 * @brief StageViewport class
 * @version 0.1
 * @date 2024-01-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef STAGEVIEWPORT_HPP
#define STAGEVIEWPORT_HPP

#include "types.hpp"

class StageViewport {
public:
	typedef double ZoomType;
private:
	RectF m_srcRect; 
	Size2dF m_dstSize;
	Size2dF m_stageSize;
	PointF m_dragBegin;
	PointF m_srcRectBegin;
	bool m_isDragging;
	ZoomType m_zoom;

	void pullLeash();
public:
	static constexpr ZoomType DEFAULT_ZOOM_FACTOR = 1.2;

	StageViewport(const Size2dF& stageSize, const Size2dF& dstSize);

	void setStagePos(PointF::ValueType x, PointF::ValueType y);
	void setStagePos(const PointF& pos);
	void srcRectCenter();

	void setStageSize(Size2dF::ValueType w, Size2dF::ValueType h);
	void setStageSize(const Size2dF& size);

	PointF dstToSrc(const PointF& p);
	RectF dstToSrc(const RectF& r);
	PointF srcToDst(const PointF& p);
	RectF srcToDst(const RectF& r);

	void beginDrag(const PointF& where);
	void doDrag(const PointF& where);
	void endDrag();
	bool isDrag();

	void setZoom(const PointF& towards, ZoomType newZoom);
	void setZoom(ZoomType newZoom);
	void zoomIn(const PointF& towards, ZoomType factor = DEFAULT_ZOOM_FACTOR);
	void zoomIn(ZoomType factor = DEFAULT_ZOOM_FACTOR);
	void zoomOut(const PointF& towards, ZoomType factor = DEFAULT_ZOOM_FACTOR);
	void zoomOut(ZoomType factor = DEFAULT_ZOOM_FACTOR);

	RectF getSrcRect() const;
	ZoomType getZoom() const;
	Matrix3x3 getProjectionToScreenMatrix() const;
	Matrix3x3 getProjectionToWorkspaceMatrix() const;
};

#endif // STAGEVIEWPORT_HPP
