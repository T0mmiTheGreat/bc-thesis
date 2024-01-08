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

void MainMenuController::startedEvent()
{
	GeneralControllerBase::startedEvent();

	Size2d paintareaSize = sysProxy->getPaintAreaSize();

	for (auto& btn : m_menuBtns) {
		btn = std::make_unique<MainMenuItemSprite>(sysProxy);
	}
	m_title = std::make_unique<MainMenuTitleSprite>(sysProxy);

	auto& playBtn = m_menuBtns[MENU_PLAY_BTN_IDX];
	playBtn->setText("Play");
	Size2d playBtnSize = playBtn->getSize();
	playBtn->setPos(
		(paintareaSize.w - playBtnSize.w) / 2,
		MENU_TOP
	);

	auto& quitBtn = m_menuBtns[MENU_QUIT_BTN_IDX];
	quitBtn->setText("Quit");
	Size2d quitBtnSize = quitBtn->getSize();
	quitBtn->setPos(
		(paintareaSize.w - quitBtnSize.w) / 2,
		playBtn->getY() + playBtnSize.h + MENU_ITEM_SPACING
	);

	Size2d titleSize = m_title->getSize();
	m_title->setPos((paintareaSize.w - titleSize.w) / 2, 70);
}

void MainMenuController::mouseMoveEvent(int x, int y)
{
	Point mousePt(x, y);

	for (auto& btn : m_menuBtns) {
		if (btn->getBounds().containsPoint(mousePt)) {
			btn->setCostume(MainMenuItemSprite::COSTUME_HOVER);
		} else {
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
