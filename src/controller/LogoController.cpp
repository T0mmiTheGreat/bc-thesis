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

void LogoController::startedEvent()
{
	GeneralControllerBase::startedEvent();
	m_timer.reset();
	m_opacity = 0;
	m_frameNumber = 0;
}

void LogoController::frameEvent()
{
	GeneralControllerBase::frameEvent();
	if (m_timer.isLap()) {
		m_frameNumber++;
		if (m_frameNumber <= 0x3f) {
			m_opacity = m_frameNumber * 4;
		}
		else if (m_frameNumber <= 100) {
			m_opacity = 0xff;
		}
		else if (m_frameNumber <= 100 + 0x3f) {
			m_opacity = 0xff - ((m_frameNumber - 100) * 4);
		}
		else {
			m_opacity = 0;
			// finishedEvent();
		}
		sysProxy->invalidateRect();
	}
}

void LogoController::paintEvent(std::shared_ptr<ICanvas> canvas, Rect& invalidRect)
{
	GeneralControllerBase::paintEvent(canvas, invalidRect);
	canvas->setFillingColor(Color(m_opacity, m_opacity, m_opacity, 0xff));
	canvas->fillCircle(195, 100, 25);
	canvas->fillCircle(480 - 195, 100, 25);
}
