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

std::shared_ptr<IControllerChild> LogoController::createReplacement()
{
	return ControllerFactory::createMainMenuController(sysProxy);
}

void LogoController::startedEvent()
{
	GeneralControllerBase::startedEvent();

	// Create logo sprite
	m_sprite = std::make_unique<LogoSprite>(sysProxy);

	// Sprite position calculations
	Size2d winSize = sysProxy->getPaintAreaSize(); // Paint area size
	Size2d spriteSize = m_sprite->getSize();       // Sprite size
	Size2d remSize = winSize - spriteSize;         // Remaining size (paint area minus sprite)

	// Set position (window center)
	m_sprite->setPos(remSize.w / 2, remSize.h / 2);
	// Animate
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

void LogoController::paintEvent(std::shared_ptr<ICanvas> canvas, const Rect& invalidRect)
{
	GeneralControllerBase::paintEvent(canvas, invalidRect);
	m_sprite->repaint(canvas, invalidRect);
}
