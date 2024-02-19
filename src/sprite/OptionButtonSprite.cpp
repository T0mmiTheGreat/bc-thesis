/**
 * @file OptionButtonSprite.cpp
 * @author Tomáš Ludrovan
 * @brief OptionButtonSprite class
 * @version 0.1
 * @date 2024-02-12
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "sprite/OptionButtonSprite.hpp"

OptionButtonSprite::OptionButtonSprite(std::shared_ptr<IPaintingProxy> paintingProxy)
	: SpriteBase(paintingProxy)
	, BoundedSpriteBase(paintingProxy)
	, PositionedSpriteBase(paintingProxy)
	, m_costume{COSTUME_NORMAL}
{}

Size2d OptionButtonSprite::getSize()
{
	return Size2d(RECT_WIDTH_SCALED, RECT_HEIGHT_SCALED);
}

void OptionButtonSprite::repaintAsVisible(std::shared_ptr<ICanvas> canvas, const Rect& invalidRect)
{
	(void)invalidRect;

	Size2d spriteSize = getSize();

	// Bounds
	canvas->setStrokingColor(Color::white());
	int rectW = ((m_costume == COSTUME_HOVER) ? RECT_WIDTH_SCALED : RECT_WIDTH_NORMAL);
	int rectH = ((m_costume == COSTUME_HOVER) ? RECT_HEIGHT_SCALED : RECT_HEIGHT_NORMAL);
	int rectX = x + ((spriteSize.w - rectW) / 2);
	int rectY = y + ((spriteSize.h - rectH) / 2);
	canvas->strokeRectangle(rectX, rectY, rectW, rectH);

	// Text
	canvas->setFillingColor(Color::white());
	Size2d textSize = paintingProxy->getTextSize(m_text, FONT);
	int textX = x + ((spriteSize.w - textSize.w) / 2);
	int textY = y + ((spriteSize.h - textSize.h) / 2);
	canvas->fillText(textX, textY, m_text, FONT);
}

const std::string& OptionButtonSprite::getText() const
{
	return propertyGetter(m_text);
}

void OptionButtonSprite::setText(const std::string& value)
{
	propertySetterComparable(m_text, value);
}

OptionButtonSprite::Costume OptionButtonSprite::getCostume() const
{
	return propertyGetter(m_costume);
}

void OptionButtonSprite::setCostume(OptionButtonSprite::Costume value)
{
	propertySetterComparable(m_costume, value);
}
