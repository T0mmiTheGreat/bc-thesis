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

void LogoController::startedEvent()
{
	GeneralControllerBase::startedEvent();
	m_sprite = std::make_unique<LogoSprite>();
}

void LogoController::frameEvent()
{
	GeneralControllerBase::frameEvent();
	m_sprite->frameEvent();
}

void LogoController::paintEvent(std::shared_ptr<ICanvas> canvas, Rect& invalidRect)
{
	GeneralControllerBase::paintEvent(canvas, invalidRect);
	m_sprite->repaint(canvas, invalidRect);
}
