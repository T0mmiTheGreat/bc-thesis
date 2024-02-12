/**
 * @file TextBoxSprite.cpp
 * @author Tomáš Ludrovan
 * @brief TextBoxSprite class
 * @version 0.1
 * @date 2024-02-12
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "sprite/TextBoxSprite.hpp"

void TextBoxSprite::repaintAsVisible(std::shared_ptr<ICanvas> canvas,
	const Rect& invalidRect)
{
	(void)invalidRect;

	Rect bounds = getBounds();

	canvas->setFillingColor(m_color);
	canvas->fillText(bounds.x, bounds.y, m_text, m_font);
}

TextBoxSprite::TextBoxSprite(std::shared_ptr<IPaintingProxy> paintingProxy)
	: SpriteBase(paintingProxy)
	, BoundedSpriteBase(paintingProxy)
	, m_textRect()
	, m_text()
	, m_font{FONT_TAHOMA_16}
	, m_color()
{}

Rect TextBoxSprite::getBounds()
{
	Rect textRect(0, 0, paintingProxy->getTextSize(m_text, m_font));
	textRect.centerAt(m_textRect);
	return textRect;
}

const Rect& TextBoxSprite::getTextRect() const
{
	return propertyGetter(m_textRect);
}

void TextBoxSprite::setTextRect(const Rect& value)
{
	propertySetterComparable(m_textRect, value);
}

const std::string& TextBoxSprite::getText() const
{
	return propertyGetter(m_text);
}

void TextBoxSprite::setText(const std::string& value)
{
	propertySetterComparable(m_text, value);
}

FontId TextBoxSprite::getFont() const
{
	return propertyGetter(m_font);
}

void TextBoxSprite::setFont(FontId value)
{
	propertySetterComparable(m_font, value);
}

const Color& TextBoxSprite::getColor() const
{
	return propertyGetter(m_color);
}

void TextBoxSprite::setColor(const Color& value)
{
	propertySetterComparable(m_color, value);
}
