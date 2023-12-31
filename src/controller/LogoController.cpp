/**
 * @file LogoController.cpp
 * @author Tomáš Ludrovan
 * @brief LogoController class
 * @version 0.1
 * @date 2023-12-18
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "controller/LogoController.hpp"

#include <memory>

#include "controller/ControllerFactory.hpp"

std::unique_ptr<IControllerChild> LogoController::createReplacement()
{
	return ControllerFactory::createMainMenuController();
}

void LogoController::startedEvent()
{
	GeneralControllerBase::startedEvent();
	m_sprite = std::make_unique<LogoSprite>(sysProxy);

	Size2d winSize = sysProxy->getPaintAreaSize();
	Size2d spriteSize = m_sprite->getSize();
	Size2d remSize = winSize - spriteSize;

	m_sprite->setPos(remSize.w / 2, remSize.h / 2);
	m_sprite->startAnimation();
}

void LogoController::loopEvent()
{
	GeneralControllerBase::loopEvent();
	m_sprite->loopEvent();
	if (!m_sprite->isAnimationRunning()) {
		finishedEvent();
	}
}

void LogoController::paintEvent(std::shared_ptr<ICanvas> canvas, Rect& invalidRect)
{
	GeneralControllerBase::paintEvent(canvas, invalidRect);
	m_sprite->repaint(canvas, invalidRect);
}
