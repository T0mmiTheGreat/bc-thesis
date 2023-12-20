/**
 * @file MainMenuSprite.cpp
 * @author Tomáš Ludrovan
 * @brief MainMenuSprite class
 * @version 0.1
 * @date 2023-12-20
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "sprite/MainMenuSprite.hpp"

MainMenuSprite::MainMenuSprite()
	: SpriteBase()
{
	m_playBtn.setText("Play");
	m_quitBtn.setText("Quit");

	posChangedEvent(0, 0, 0, 0);
}

void MainMenuSprite::posChangedEvent(int oldX, int oldY, int newX, int newY)
{
	SpriteBase::posChangedEvent(oldX, oldY, newX, newY);
	int itemY = newY;
	for (auto menuItem : getMenuItems()) {
		menuItem->setPos(newX, itemY);
		itemY += menuItem->getH() + ITEM_SPACING;
	}
}

std::array<MainMenuItemSprite*, MainMenuSprite::ITEM_COUNT> MainMenuSprite::getMenuItems()
{
	return std::array<MainMenuItemSprite*, ITEM_COUNT>{
		&m_playBtn,
		&m_quitBtn
	};
}

Size2d MainMenuSprite::getSize()
{
	Rect menuRect(x, y, 0, 0);
	for (auto menuItem : getMenuItems()) {
		menuRect += menuItem->getBounds();
	}
	return menuRect.getSize();
}

void MainMenuSprite::repaint(std::shared_ptr<ICanvas> canvas, Rect& invalidRect)
{
	for (auto menuItem : getMenuItems()) {
		menuItem->repaint(canvas, invalidRect);
	}
}

void MainMenuSprite::mouseMoveEvent(int x, int y)
{
	for (auto menuItem : getMenuItems()) {
		menuItem->mouseMoveEvent(x, y);
	}
}
