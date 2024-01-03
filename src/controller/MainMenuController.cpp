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

	Size2d paintareaSize = sysProxy->getPaintAreaSize();

	m_mainMenu = std::make_unique<MainMenuSprite>();
	m_title = std::make_unique<MainMenuTitleSprite>();

	Size2d menuSize = m_mainMenu->getSize();
	m_mainMenu->setPos((paintareaSize.w - menuSize.w) / 2, 230);

	Size2d titleSize = m_title->getSize();
	m_title->setPos((paintareaSize.w - titleSize.w) / 2, 70);
}

void MainMenuController::mouseMoveEvent(int x, int y)
{
	m_mainMenu->mouseMoveEvent(x, y);
}

void MainMenuController::loopEvent()
{
	GeneralControllerBase::loopEvent();
	m_mainMenu->loopEvent();
	m_title->loopEvent();
}

void MainMenuController::paintEvent(std::shared_ptr<ICanvas> canvas, Rect& invalidRect)
{
	GeneralControllerBase::paintEvent(canvas, invalidRect);
	m_mainMenu->repaint(canvas, invalidRect);
	m_title->repaint(canvas, invalidRect);
}
