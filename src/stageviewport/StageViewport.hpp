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

/**
 * @brief Class which takes care of drag and zoom logic.
 */
class StageViewport {
public:
	typedef double ZoomType;
private:
	// Projection of the workspace to the stage space
	RectF m_srcRect;
	// Size of the workspace
	Size2dF m_dstSize;
	Size2dF m_stageSize;
	// The drag origin, within the screen space
	PointF m_dragBegin;
	// The position of src rect at the moment when the dragging started
	PointF m_srcRectBegin;
	bool m_isDraggingSelected;
	ZoomType m_zoom;

	/**
	 * @brief Ensures that the user doesn't go too far away from the stage area.
	 * 
	 * @details The workspace are and the stage area must have at least one
	 *          common point -- a corner. For example:
	 * 
	 *          +------+             +------+
	 *          |  wa  |             |  sa  +------+
	 *          +------+------+      |      |  wa  |
	 *                 |  sa  |      +------+------+
	 *                 |      |
	 *                 +------+
	 */
	void pullLeash();
public:
	/**
	 * @brief The value to multiply/divide the zoom by when zooming in/out.
	 * 
	 * @details This seems to be a good factor for one scroll of the mouse
	 *          wheel, but some devices might have different sensitivity, so
	 *          a different factor value may be chosen.
	 */
	static constexpr ZoomType DEFAULT_ZOOM_FACTOR = 1.2;

	/**
	 * @brief Constructs a new StageViewport object.
	 * 
	 * @param stageSize Size of the stage.
	 * @param dstSize Size of the workspace.
	 */
	StageViewport(const Size2dF& stageSize, const Size2dF& dstSize);

	/**
	 * @brief Sets the stage position within the workspace area.
	 */
	void setStagePos(PointF::ValueType x, PointF::ValueType y);
	/**
	 * @brief Sets the stage position within the workspace area.
	 */
	void setStagePos(const PointF& pos);
	/**
	 * @brief Sets the stage position to the center of the workspace area.
	 */
	void stageCenter();

	/**
	 * @brief Changes the size of the stage.
	 */
	void setStageSize(Size2dF::ValueType w, Size2dF::ValueType h);
	/**
	 * @brief Changes the size of the stage.
	 */
	void setStageSize(const Size2dF& size);

	/**
	 * @brief Projects the point from the screen space to the stage space.
	 */
	PointF screenToStage(const PointF& p) const;
	/**
	 * @brief Projects the rectangle from the screen space to the stage space.
	 */
	RectF screenToStage(const RectF& r) const;
	/**
	 * @brief Projects the point from the stage space to the screen space.
	 */
	PointF stageToScreen(const PointF& p) const;
	/**
	 * @brief Projects the rectangle from the stage space to the screen space.
	 */
	RectF stageToScreen(const RectF& r) const;

	/**
	 * @brief Begins the dragging operation.
	 * 
	 * @param where The point where the dragging started within the screen
	 *              space. This can be completely arbitrary point as long as
	 *              the way it's obtained is the same as for the subsequent
	 *              calls to `doDrag()`. For example, you can pass the absolute
	 *              position of the mouse if you also pass it to the `doDrag()`
	 *              afterwards.
	 * 
	 * @remark If a dragging operation is under way, this will start a new one.
	 */
	void beginDrag(const PointF& where);
	/**
	 * @brief Performs dragging.
	 * 
	 * @param where The drag destination. It should be obtained the same way as
	 *              the `where` parameter of `beginDrag()`.
	 * 
	 * @remark No-op if there's no dragging operation under way.
	 */
	void doDrag(const PointF& where);
	/**
	 * @brief Finishes the dragging operation.
	 * 
	 * @remark No-op if there's no dragging operation under way.
	 */
	void endDrag();
	/**
	 * @brief Checks whether a dragging operation is under way.
	 */
	bool isDrag();

	/**
	 * @brief Moves the viewport without performing a dragging operation.
	 * 
	 * @param dx Move in horizontal direction (positive to the right).
	 * @param dy Move in vertical direction (positive downwards).
	 * 
	 * @todo The positive/negative directions may be actually wrong.
	 */
	void scroll(double dx, double dy);

	/**
	 * @brief Sets zoom value.
	 * 
	 * @param towards The point in the screen space to zoom towards.
	 * @param newZoom New zoom value. 1 means normal size, 2 means double size,
	 *                etc. Must not be 0.
	 */
	void setZoom(const PointF& towards, ZoomType newZoom);
	/**
	 * @brief Zooms towards or away from the center of the workspace.
	 * 
	 * @param newZoom New zoom value. 1 means normal size, 2 means double size,
	 *                etc. Must not be 0.
	 */
	void setZoom(ZoomType newZoom);
	/**
	 * @brief Increases zoom value.
	 * 
	 * @param towards The point in the screen space to zoom towards.
	 * @param factor The zoom factor. Zooming in is multiplying the zoom value
	 *               by a factor. A factor of 2 makes every object twice as big
	 *               as with the previous zoom value. Values between 0 and 1
	 *               actually cause zoom out. Must not be 0.
	 */
	void zoomIn(const PointF& towards, ZoomType factor = DEFAULT_ZOOM_FACTOR);
	/**
	 * @brief Zooms towards the center of the workspace.
	 * 
	 * @param factor The zoom factor. Zooming in is multiplying the zoom value
	 *               by a factor. A factor of 2 makes every object twice as big
	 *               as with the previous zoom value. Values between 0 and 1
	 *               actually cause zoom out. Must not be 0.
	 */
	void zoomIn(ZoomType factor = DEFAULT_ZOOM_FACTOR);
	/**
	 * @brief Decreases zoom value.
	 * 
	 * @param towards The point in the screen space to zoom away from.
	 * @param factor The zoom divisor. Zooming out is dividing the zoom value
	 *               by a divisor. A divisor of 2 makes every object twice as
	 *               small as with the previous zoom value. Values between
	 *               0 and 1 actually cause zoom in. Must not be 0.
	 */
	void zoomOut(const PointF& towards, ZoomType factor = DEFAULT_ZOOM_FACTOR);
	/**
	 * @brief Zooms away from the center of the workspace.
	 * 
	 * @param factor The zoom divisor. Zooming out is dividing the zoom value
	 *               by a divisor. A divisor of 2 makes every object twice as
	 *               small as with the previous zoom value. Values between
	 *               0 and 1 actually cause zoom in. Must not be 0.
	 */
	void zoomOut(ZoomType factor = DEFAULT_ZOOM_FACTOR);

	/**
	 * @brief Getter for the src rect read-only property.
	 * 
	 * @details Src rect is the projection of the workspace stage space.
	 */
	RectF getSrcRect() const;
	/**
	 * @brief Getter for the zoom property.
	 */
	ZoomType getZoom() const;
	/**
	 * @brief Creates a transformation matrix for conversion from stage space to
	 *        workspace.
	 */
	Matrix3x3 getProjectionToScreenMatrix() const;
	/**
	 * @brief Creates a transformation matrix for conversion from workspace to
	 *        stage space.
	 */
	Matrix3x3 getProjectionToStageMatrix() const;
};

#endif // STAGEVIEWPORT_HPP
