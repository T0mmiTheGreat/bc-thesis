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
	, m_costume{COSTUME_NORMAL}
	, m_solidsFrequency{0}
	, m_xSpacing{0.0}
	, m_ySpacing{0.0}
	, m_size()
{
}

Size2d EditorWorkspaceGridSprite::getSize()
{
	return m_size;
}

EditorWorkspaceGridSprite::Costume EditorWorkspaceGridSprite::getCostume() const
{
	return m_costume;
}

void EditorWorkspaceGridSprite::setCostume(
	EditorWorkspaceGridSprite::Costume value)
{
	if (m_costume != value) {
		invalidate();
		m_costume = value;
		invalidate();
	}
}

void EditorWorkspaceGridSprite::repaintCostumeNormal(
	std::shared_ptr<ICanvas> canvas, const Rect& invalidRect)
{
	repaintColor(GRID_BORDER_COLOR_NORMAL, GRID_CELL_COLOR_NORMAL, canvas,
		invalidRect);
}

void EditorWorkspaceGridSprite::repaintCostumeBad(
	std::shared_ptr<ICanvas> canvas, const Rect& invalidRect)
{
	repaintColor(GRID_BORDER_COLOR_BAD, GRID_CELL_COLOR_BAD, canvas,
		invalidRect);
}

void EditorWorkspaceGridSprite::repaintColor(
	const Color& gridBorderColor, const Color& gridCellColor,
	std::shared_ptr<ICanvas> canvas, const Rect& invalidRect)
{
	Rect bounds = getBounds();
	Point bottomRight = bounds.getBottomRight();

	double iRectLeft   = invalidRect.x;
	double iRectTop    = invalidRect.y;
	double iRectRight  = invalidRect.getRight();
	double iRectBottom = invalidRect.getBottom();

	// Border
	canvas->setStrokingColor(gridBorderColor);
	canvas->strokeRectangle(bounds.x, bounds.y, bounds.w, bounds.h);

	// Set color for cells
	canvas->setStrokingColor(gridCellColor);

	// Vertical lines
	if (m_xSpacing != 0.0) {
		unsigned lineIdx = 0;

		for (double x = bounds.x + m_xSpacing; x < bottomRight.x; x += m_xSpacing) {
			lineIdx++;

			// Paint only if the line is within invalid rect
			if (iRectLeft <= x && x <= iRectRight) {
				// If `m_solidsFrequency == 0`, no solid lines should be painted,
				// and they won't, because `lineIdx` will not be 0 here (unless
				// integer overflow...)
				if (lineIdx == m_solidsFrequency) {
					// Solid line

					canvas->strokeLine(static_cast<int>(x), bounds.y,
						static_cast<int>(x), bottomRight.y);
				} else {
					// Dashed line

					canvas->dashedLine(static_cast<int>(x), bounds.y,
						static_cast<int>(x), bottomRight.y);
				}
			}

			// Reset counter
			if (lineIdx == m_solidsFrequency) {
				lineIdx = 0;
			}
		}
	}

	// Horizontal lines
	if (m_ySpacing != 0.0) {
		unsigned lineIdx = 0;

		for (double y = bounds.y + m_ySpacing; y < bottomRight.y; y += m_ySpacing) {
			lineIdx++;

			// Paint only if the line is within invalid rect
			if (iRectTop <= y && y <= iRectBottom) {
				// If `m_solidsFrequency == 0`, no solid lines should be painted,
				// and they won't, because `lineIdx` will not be 0 here (unless
				// integer overflow...)
				if (lineIdx == m_solidsFrequency) {
					// Solid line

					canvas->strokeLine(bounds.x, static_cast<int>(y),
						bottomRight.x, static_cast<int>(y));
				} else {
					// Dashed line

					canvas->dashedLine(bounds.x, static_cast<int>(y),
						bottomRight.x, static_cast<int>(y));
				}
			}

			// Reset counter
			if (lineIdx == m_solidsFrequency) {
				lineIdx = 0;
			}
		}
	}
}

void EditorWorkspaceGridSprite::repaintAsVisible(std::shared_ptr<ICanvas> canvas,
	const Rect& invalidRect)
{
	switch (m_costume) {
		case COSTUME_NORMAL:
			repaintCostumeNormal(canvas, invalidRect);
			break;
		case COSTUME_BAD:
			repaintCostumeBad(canvas, invalidRect);
			break;
	}
}

unsigned EditorWorkspaceGridSprite::getSolidsFrequency() const
{
	return m_solidsFrequency;
}

void EditorWorkspaceGridSprite::setSolidsFrequency(unsigned value)
{
	if (m_solidsFrequency != value) {
		invalidate();
		m_solidsFrequency = value;
		invalidate();
	}
}

void EditorWorkspaceGridSprite::setSize(const Size2d& value)
{
	Size2d s(value);
	setSize(std::move(s));
}

void EditorWorkspaceGridSprite::setSize(Size2d&& value)
{
	propertySetterComparable(m_size, value);
}

double EditorWorkspaceGridSprite::getXSpacing() const
{
	return m_xSpacing;
}

void EditorWorkspaceGridSprite::setXSpacing(double value)
{
	if (m_xSpacing != value) {
		invalidate();
		m_xSpacing = value;
		invalidate();
	}
}

double EditorWorkspaceGridSprite::getYSpacing() const
{
	return m_ySpacing;
}

void EditorWorkspaceGridSprite::setYSpacing(double value)
{
	if (m_ySpacing != value) {
		invalidate();
		m_ySpacing = value;
		invalidate();
	}
}
