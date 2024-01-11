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

MainMenuItemSprite::MainMenuItemSprite(std::shared_ptr<IPaintingProxy> paintingProxy)
	: SpriteBase(paintingProxy)
	, m_costume{COSTUME_NORMAL}
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

	// We might have repainted an area bigger than invalidRect
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
		paintingProxy->invalidateRect(getBounds());
	}
}

MainMenuItemSprite::Costume MainMenuItemSprite::getCostume() const
{
	return m_costume;
}

void MainMenuItemSprite::setCostume(MainMenuItemSprite::Costume costume)
{
	if (m_costume != costume) {
		// Paint over the current costume
		paintingProxy->invalidateRect(getBounds());
		m_costume = costume;
		// Paint the new costume
		paintingProxy->invalidateRect(getBounds());
	}
}
