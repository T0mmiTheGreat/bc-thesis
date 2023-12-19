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

	m_playBtn->setPos(50, 50);
	m_playBtn->setText("Play");
}

void MainMenuController::mouseMoveEvent(int x, int y)
{
	m_playBtn->mouseMoveEvent(x, y);
}

void MainMenuController::frameEvent()
{
	GeneralControllerBase::frameEvent();
	m_playBtn->frameEvent();
}

void MainMenuController::paintEvent(std::shared_ptr<ICanvas> canvas, Rect& invalidRect)
{
	GeneralControllerBase::paintEvent(canvas, invalidRect);
	m_playBtn->repaint(canvas, invalidRect);
}
