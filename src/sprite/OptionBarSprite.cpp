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
	, BoundedSpriteBase(paintingProxy)
	, PositionedSpriteBase(paintingProxy)
	, m_barWidth{0}
	, m_barHeight{0}
{
}

Size2d OptionBarSprite::getSize()
{
	Size2d paintAreaSize = paintingProxy->getPaintAreaSize();
	Size2d res(
		(m_barWidth  > 0 ? m_barWidth  : paintAreaSize.w + m_barWidth  - getX()),
		(m_barHeight > 0 ? m_barHeight : paintAreaSize.h + m_barHeight - getY())
	);

	return res;
}

void OptionBarSprite::repaintAsVisible(std::shared_ptr<ICanvas> canvas, const Rect& invalidRect)
{
	(void)invalidRect;

	Rect bounds = getBounds();
	// Top left
	Point tl(bounds.x, bounds.y);
	// Top right
	Point tr(bounds.getRight(), bounds.y);
	// Bottom right
	Point br(bounds.getBottomRight());
	// Bottom left
	Point bl(bounds.x, bounds.getBottom());

	// Set graphics parameters
	canvas->setFillingColor(m_fColor);
	canvas->setStrokingColor(m_sColor);
	canvas->setStrokeWidth(1.0);

	// Fill
	canvas->fillRectangle(bounds.x, bounds.y, bounds.w, bounds.h);
	// Stroke
	if (m_borders.left) canvas->strokeLine(tl.x, tl.y, bl.x, bl.y);
	if (m_borders.top) canvas->strokeLine(tl.x, tl.y, tr.x, tr.y);
	if (m_borders.right) canvas->strokeLine(tr.x, tr.y, br.x, br.y);
	if (m_borders.bottom) canvas->strokeLine(bl.x, bl.y, br.x, br.y);
}

int OptionBarSprite::getBarWidth() const
{
	return propertyGetter(m_barWidth);
}

void OptionBarSprite::setBarWidth(int value)
{
	propertySetterComparable(m_barWidth, value);
}

int OptionBarSprite::getBarHeight() const
{
	return propertyGetter(m_barHeight);
}

void OptionBarSprite::setBarHeight(int value)
{
	propertySetterComparable(m_barHeight, value);
}

const OptionBarSprite::Borders& OptionBarSprite::getBorders() const
{
	return propertyGetter(m_borders);
}

void OptionBarSprite::setBorders(const OptionBarSprite::Borders& value)
{
	OptionBarSprite::Borders v = value;
	setBorders(std::move(v));
}

void OptionBarSprite::setBorders(OptionBarSprite::Borders&& value)
{
	propertySetterComparable(m_borders, value);
}

void OptionBarSprite::setBorders(bool left, bool top, bool right, bool bottom)
{
	setBorders(Borders(left, top, right, bottom));
}

const Color& OptionBarSprite::getFillingColor() const
{
	return propertyGetter(m_fColor);
}

void OptionBarSprite::setFillingColor(const Color& value)
{
	propertySetterComparable(m_fColor, value);
}

const Color& OptionBarSprite::getStrokingColor() const
{
	return propertyGetter(m_sColor);
}

void OptionBarSprite::setStrokingColor(const Color& value)
{
	propertySetterComparable(m_sColor, value);
}
