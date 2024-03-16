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
	, m_size{BUTTON_SIZE_MENU}
{}

Size2d OptionButtonSprite::getSize()
{
	return propertyGetter(m_size);
}

void OptionButtonSprite::setSize(const Size2d& value)
{
	propertySetterComparable(m_size, value);
}

Size2d OptionButtonSprite::getSizeUnscaled() const
{
	static constexpr int MUL_V = 5, DIV_V = 6;

	Size2d res((m_size.w * MUL_V) / DIV_V, (m_size.h * MUL_V) / DIV_V);
	return res;
}

void OptionButtonSprite::repaintAsVisible(std::shared_ptr<ICanvas> canvas,
	const Rect& invalidRect)
{
	(void)invalidRect;

	// Bounds
	canvas->setStrokingColor(Color::white());
	Rect hitboxRect(getX(), getY(), getSize()), boundsRect;
	switch (m_costume) {
		case COSTUME_NORMAL:
			boundsRect = Rect(0, 0, getSizeUnscaled());
			boundsRect.centerAt(hitboxRect);
			break;
		case COSTUME_HOVER:
			boundsRect = hitboxRect;
			break;
	}
	canvas->strokeRectangle(boundsRect.x, boundsRect.y, boundsRect.w,
		boundsRect.h);

	// Text
	canvas->setFillingColor(Color::white());
	Size2d textSize = paintingProxy->getTextSize(m_text, FONT);
	int textX = x + ((hitboxRect.w - textSize.w) / 2);
	int textY = y + ((hitboxRect.h - textSize.h) / 2);
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
