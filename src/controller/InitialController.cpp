/**
 * @file InitialController.cpp
 * @author Tomáš Ludrovan
 * @brief InitialController class
 * @version 0.1
 * @date 2023-12-03
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "controller/InitialController.hpp"

std::unique_ptr<IController> InitialController::runController()
{
	return GeneralControllerBase::runController();
}

void InitialController::keyDownEvent(KeyCode key)
{
	switch (key) {
		case KEY_LEFT_ARROW:
			spriteX -= 3;
			break;
		case KEY_UP_ARROW:
			spriteY -= 3;
			break;
		case KEY_RIGHT_ARROW:
			spriteX += 3;
			break;
		case KEY_DOWN_ARROW:
			spriteY += 3;
			break;
	}
	sysProxy->invalidateRect();
}

void InitialController::startEvent()
{
	sysProxy->invalidateRect();
}

void InitialController::paintEvent(std::shared_ptr<ICanvas> canvas, Rect & invalidRect)
{
	canvas->setFillingColor(Color(0xff, 0, 0, 0xff));
	canvas->drawCircle(spriteX, spriteY, spriteR);
}
