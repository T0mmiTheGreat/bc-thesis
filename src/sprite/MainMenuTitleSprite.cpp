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

MainMenuTitleSprite::MainMenuTitleSprite()
	: SpriteBase()
{}

Size2d MainMenuTitleSprite::getSize()
{
	return sysProxy->getTextSize(TITLE_TEXT, TITLE_FONT);
}

void MainMenuTitleSprite::repaint(std::shared_ptr<ICanvas> canvas, Rect& invalidRect)
{
	canvas->setFillingColor(Color(0xff, 0xff, 0xff, 0xff));
	canvas->fillText(x, y, TITLE_TEXT, TITLE_FONT);
	invalidRect += getBounds();
}
