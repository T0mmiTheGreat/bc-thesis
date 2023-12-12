/**
 * @file InGameController.cpp
 * @author Tomáš Ludrovan
 * @brief InGameController class
 * @version 0.1
 * @date 2023-12-05
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "controller/InGameController.hpp"

void InGameController::startedEvent()
{
	m_timer.reset();
	sysProxy->invalidateRect();
}

void InGameController::frameEvent()
{
	if (m_timer.isLap()) {
		if (sysProxy->isKeyPressed(KEY_LEFT_ARROW)) {
			if (sysProxy->isKeyPressed(KEY_UP_ARROW)) {
				spriteX -= 2;
				spriteY -= 2;
			}
			else if (sysProxy->isKeyPressed(KEY_DOWN_ARROW)) {
				spriteX -= 2;
				spriteY += 2;
			}
			else {
				spriteX -= 3;
			}
			sysProxy->invalidateRect();
		}
		else if (sysProxy->isKeyPressed(KEY_RIGHT_ARROW)) {
			if (sysProxy->isKeyPressed(KEY_UP_ARROW)) {
				spriteX += 2;
				spriteY -= 2;
			}
			else if (sysProxy->isKeyPressed(KEY_DOWN_ARROW)) {
				spriteX += 2;
				spriteY += 2;
			}
			else {
				spriteX += 3;
			}
			sysProxy->invalidateRect();
		}
		else if (sysProxy->isKeyPressed(KEY_UP_ARROW)) {
			spriteY -= 3;
			sysProxy->invalidateRect();
		}
		else if (sysProxy->isKeyPressed(KEY_DOWN_ARROW)) {
			spriteY += 3;
			sysProxy->invalidateRect();
		}
	}
}

void InGameController::paintEvent(std::shared_ptr<ICanvas> canvas, Rect& invalidRect)
{
	canvas->setFillingColor(Color(0xff, 0, 0, 0xff));
	canvas->drawCircle(spriteX, spriteY, spriteR);
}
