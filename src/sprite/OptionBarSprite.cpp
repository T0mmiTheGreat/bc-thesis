/**
 * @file OptionBarSprite.cpp
 * @author Tomáš Ludrovan
 * @brief OptionBarSprite class
 * @version 0.1
 * @date 2024-01-30
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "sprite/OptionBarSprite.hpp"

OptionBarSprite::OptionBarSprite(std::shared_ptr<IPaintingProxy> paintingProxy)
	: SpriteBase(paintingProxy)
	, m_barWidth{0}
	, m_barHeight{0}
{
}

Size2d OptionBarSprite::getSize()
{
	Size2d paintAreaSize = paintingProxy->getPaintAreaSize();
	Size2d res(
		(m_barWidth > 0 ? m_barWidth : paintAreaSize.w + m_barWidth - getX()),
		(m_barHeight > 0 ? m_barHeight : paintAreaSize.h + m_barHeight - getY())
	);

	return res;
}

void OptionBarSprite::repaint(std::shared_ptr<ICanvas> canvas, Rect& invalidRect)
{
	Rect bounds = getBounds();
	Point tl(bounds.x, bounds.y);
	Point tr(bounds.getRight(), bounds.y);
	Point br(bounds.getBottomRight());
	Point bl(bounds.x, bounds.getBottom());

	canvas->setFillingColor(m_fColor);
	canvas->setStrokingColor(m_sColor);
	canvas->setStrokeWidth(1.0);

	canvas->fillRectangle(bounds.x, bounds.y, bounds.w, bounds.h);
	if (m_borders.left) canvas->strokeLine(tl.x, tl.y, bl.x, bl.y);
	if (m_borders.top) canvas->strokeLine(tl.x, tl.y, tr.x, tr.y);
	if (m_borders.right) canvas->strokeLine(tr.x, tr.y, br.x, br.y);
	if (m_borders.bottom) canvas->strokeLine(bl.x, bl.y, br.x, br.y);
}

int OptionBarSprite::getBarWidth() const
{
	return m_barWidth;
}

void OptionBarSprite::setBarWidth(int value)
{
	if (m_barWidth != value) {
		invalidateBounds();
		m_barWidth = value;
		invalidateBounds();
	}
}

int OptionBarSprite::getBarHeight() const
{
	return m_barHeight;
}

void OptionBarSprite::setBarHeight(int value)
{
	if (m_barHeight != value) {
		invalidateBounds();
		m_barHeight = value;
		invalidateBounds();
	}
}

const OptionBarSprite::Borders& OptionBarSprite::getBorders() const
{
	return m_borders;
}

void OptionBarSprite::setBorders(const OptionBarSprite::Borders& value)
{
	OptionBarSprite::Borders v = value;
	setBorders(std::move(v));
}

void OptionBarSprite::setBorders(OptionBarSprite::Borders&& value)
{
	if (m_borders != value) {
		invalidateBounds();
		m_borders = std::move(value);
		invalidateBounds();
	}
}

void OptionBarSprite::setBorders(bool left, bool top, bool right, bool bottom)
{
	setBorders(Borders(left, top, right, bottom));
}

const Color& OptionBarSprite::getFillingColor() const
{
	return m_fColor;
}

void OptionBarSprite::setFillingColor(const Color& value)
{
	if (m_fColor != value) {
		invalidateBounds();
		m_fColor = value;
		invalidateBounds();
	}
}

const Color& OptionBarSprite::getStrokingColor() const
{
	return m_sColor;
}

void OptionBarSprite::setStrokingColor(const Color& value)
{
	if (m_sColor != value) {
		invalidateBounds();
		m_sColor = value;
		invalidateBounds();
	}
}
