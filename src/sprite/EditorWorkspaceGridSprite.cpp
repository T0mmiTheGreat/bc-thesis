/**
 * @file EditorWorkspaceGridSprite.cpp
 * @author Tomáš Ludrovan
 * @brief EditorWorkspaceGridSprite class
 * @version 0.1
 * @date 2024-01-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "sprite/EditorWorkspaceGridSprite.hpp"

EditorWorkspaceGridSprite::EditorWorkspaceGridSprite(
	std::shared_ptr<IPaintingProxy> paintingProxy)
	: SpriteBase(paintingProxy)
	, BoundedSpriteBase(paintingProxy)
	, PositionedSpriteBase(paintingProxy)
	, m_xSpacing{0.0}
	, m_ySpacing{0.0}
	, m_size()
{
}

Size2d EditorWorkspaceGridSprite::getSize()
{
	return m_size;
}

void EditorWorkspaceGridSprite::repaint(std::shared_ptr<ICanvas> canvas,
	Rect& invalidRect)
{
	Rect bounds = getBounds();
	Point bottomRight = bounds.getBottomRight();

	canvas->setStrokingColor(GRID_COLOR);
	canvas->strokeRectangle(bounds.x, bounds.y, bounds.w, bounds.h);

	// Vertical lines
	if (m_xSpacing != 0.0) {
		for (double x = bounds.x; x < bottomRight.x; x += m_xSpacing) {
			canvas->strokeLine(static_cast<int>(x), bounds.y,
				static_cast<int>(x), bottomRight.y);
		}
	}

	// Horizontal lines
	if (m_ySpacing != 0.0) {
		for (double y = bounds.y; y < bottomRight.y; y += m_ySpacing) {
			canvas->strokeLine(bounds.x, static_cast<int>(y),
				bottomRight.x, static_cast<int>(y));
		}
	}
}

void EditorWorkspaceGridSprite::setSize(const Size2d& value)
{
	Size2d s(value);
	setSize(std::move(s));
}

void EditorWorkspaceGridSprite::setSize(Size2d&& value)
{
	if (m_size != value) {
		invalidateBounds();
		m_size = std::move(value);
		invalidateBounds();
	}
}

double EditorWorkspaceGridSprite::getXSpacing() const
{
	return m_xSpacing;
}

void EditorWorkspaceGridSprite::setXSpacing(double value)
{
	if (m_xSpacing != value) {
		invalidateBounds();
		m_xSpacing = value;
		invalidateBounds();
	}
}

double EditorWorkspaceGridSprite::getYSpacing() const
{
	return m_ySpacing;
}

void EditorWorkspaceGridSprite::setYSpacing(double value)
{
	if (m_ySpacing != value) {
		invalidateBounds();
		m_ySpacing = value;
		invalidateBounds();
	}
}
