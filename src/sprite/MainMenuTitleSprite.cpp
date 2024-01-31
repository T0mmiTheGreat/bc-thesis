/**
 * @file MainMenuTitleSprite.cpp
 * @author Tomáš Ludrovan
 * @brief MainMenuTitleSprite class
 * @version 0.1
 * @date 2023-12-20
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "sprite/MainMenuTitleSprite.hpp"

MainMenuTitleSprite::MainMenuTitleSprite(std::shared_ptr<IPaintingProxy> paintingProxy)
	: SpriteBase(paintingProxy)
	, BoundedSpriteBase(paintingProxy)
	, PositionedSpriteBase(paintingProxy)
{}

Size2d MainMenuTitleSprite::getSize()
{
	return paintingProxy->getTextSize(TITLE_TEXT, TITLE_FONT);
}

void MainMenuTitleSprite::repaint(std::shared_ptr<ICanvas> canvas, Rect& invalidRect)
{
	// Paint
	canvas->setFillingColor(Color::white());
	canvas->fillText(x, y, TITLE_TEXT, TITLE_FONT);
	// We might have repainted an area bigger than invalidRect
	invalidRect += getBounds();
}
