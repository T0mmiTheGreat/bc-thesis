/**
 * @file MainMenuItemSprite.cpp
 * @author Tomáš Ludrovan
 * @brief MainMenuItemSprite class
 * @version 0.1
 * @date 2023-12-19
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "sprite/MainMenuItemSprite.hpp"

void MainMenuItemSprite::setIsScaled(bool value)
{
	if (m_isScaled != value) {
		m_isScaled = value;
		invalidateBounds();
	}
}

MainMenuItemSprite::MainMenuItemSprite()
	: SpriteBase()
{}

Size2d MainMenuItemSprite::getSize()
{
	return Size2d(RECT_WIDTH_SCALED, RECT_HEIGHT_SCALED);
}

void MainMenuItemSprite::repaint(std::shared_ptr<ICanvas> canvas, Rect& invalidRect)
{
	Size2d spriteSize = getSize();

	// Bounds
	canvas->setStrokingColor(Color::white());
	int rectW = (m_isScaled ? RECT_WIDTH_SCALED : RECT_WIDTH_NORMAL);
	int rectH = (m_isScaled ? RECT_HEIGHT_SCALED : RECT_HEIGHT_NORMAL);
	int rectX = x + ((spriteSize.w - rectW) / 2);
	int rectY = y + ((spriteSize.h - rectH) / 2);
	canvas->strokeRectangle(rectX, rectY, rectW, rectH);

	// Text
	canvas->setFillingColor(Color::white());
	Size2d textSize = sysProxy->getTextSize(m_text, FONT);
	int textX = x + ((spriteSize.w - textSize.w) / 2);
	int textY = y + ((spriteSize.h - textSize.h) / 2);
	canvas->fillText(textX, textY, m_text, FONT);

	invalidRect += getBounds();
}

const std::string& MainMenuItemSprite::getText() const
{
	return m_text;
}

void MainMenuItemSprite::setText(const std::string& value)
{
	if (m_text != value) {
		m_text = value;
		sysProxy->invalidateRect(getBounds());
	}
}

void MainMenuItemSprite::mouseMoveEvent(int x, int y)
{
	Rect spriteBounds = getBounds();
	bool isMouseHover = spriteBounds.containsPoint(Point(x, y));
	setIsScaled(isMouseHover);
}
