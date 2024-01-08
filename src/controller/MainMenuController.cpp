/**
 * @file MainMenuController.cpp
 * @author Tomáš Ludrovan
 * @brief MainMenuController class
 * @version 0.1
 * @date 2023-12-19
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "controller/MainMenuController.hpp"

#include <format>

MainMenuController::MainMenuController(std::shared_ptr<ISysProxy> sysProxy)
	: GeneralControllerBase(sysProxy)
{
}

void MainMenuController::createSprites()
{
	Size2d paintareaSize = sysProxy->getPaintAreaSize();

	// Title
	
	m_title = std::make_unique<MainMenuTitleSprite>(sysProxy);
	Size2d titleSize = m_title->getSize();
	m_title->setPos((paintareaSize.w - titleSize.w) / 2, 70);


	// Menu btns

	// Create
	for (auto& btn : m_menuBtns) {
		btn = std::make_unique<MainMenuItemSprite>(sysProxy);
	}

	// Set texts
	m_menuBtns[MENU_PLAY_BTN_IDX]->setText("Play");
	m_menuBtns[MENU_QUIT_BTN_IDX]->setText("Quit");

	// Position
	int itemY = MENU_TOP;
	for (auto& btn : m_menuBtns) {
		int btnW = btn->getW();
		btn->setPos((paintareaSize.w - btnW) / 2, itemY);
		itemY += btn->getH() + MENU_ITEM_SPACING;
	}
}

void MainMenuController::startedEvent()
{
	GeneralControllerBase::startedEvent();
	createSprites();
}

void MainMenuController::mouseMoveEvent(int x, int y)
{
	Point mousePt(x, y);

	for (auto& btn : m_menuBtns) {
		if (btn->getBounds().containsPoint(mousePt)) {
			// Mouse cursor over menu item - highlight
			btn->setCostume(MainMenuItemSprite::COSTUME_HOVER);
		} else {
			// Don't highlight
			btn->setCostume(MainMenuItemSprite::COSTUME_NORMAL);
		}
	}
}

void MainMenuController::loopEvent()
{
	GeneralControllerBase::loopEvent();
	for (auto& btn : m_menuBtns) {
		btn->loopEvent();
	}
	m_title->loopEvent();
}

void MainMenuController::paintEvent(std::shared_ptr<ICanvas> canvas, Rect& invalidRect)
{
	GeneralControllerBase::paintEvent(canvas, invalidRect);
	for (auto& btn : m_menuBtns) {
		btn->repaint(canvas, invalidRect);
	}
	m_title->repaint(canvas, invalidRect);
}
