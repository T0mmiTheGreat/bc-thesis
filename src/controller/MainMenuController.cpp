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

void MainMenuController::startedEvent()
{
	GeneralControllerBase::startedEvent();
	m_playBtn = std::make_unique<MainMenuItemSprite>();
	m_title = std::make_unique<MainMenuTitleSprite>();

	m_playBtn->setPos(50, 50);
	m_playBtn->setText("Play");

	Size2d titleSize = m_title->getSize();
	Size2d paintareaSize = sysProxy->getPaintAreaSize();
	m_title->setPos((paintareaSize.w - titleSize.w) / 2, 70);
}

void MainMenuController::mouseMoveEvent(int x, int y)
{
	m_playBtn->mouseMoveEvent(x, y);
}

void MainMenuController::frameEvent()
{
	GeneralControllerBase::frameEvent();
	m_playBtn->frameEvent();
	m_title->frameEvent();
}

void MainMenuController::paintEvent(std::shared_ptr<ICanvas> canvas, Rect& invalidRect)
{
	GeneralControllerBase::paintEvent(canvas, invalidRect);
	m_playBtn->repaint(canvas, invalidRect);
	m_title->repaint(canvas, invalidRect);
}
