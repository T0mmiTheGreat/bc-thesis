/**
 * @file InitialController.cpp
 * @author Tomáš Ludrovan
 * @brief InitialController class
 * @version 0.1
 * @date 2023-12-18
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "controller/InitialController.hpp"

#include "controller/ControllerFactory.hpp"

std::unique_ptr<IControllerChild> InitialController::createReplacement()
{
	return ControllerFactory::createInGameController();
	//return ControllerFactory::createLogoController();
}

void InitialController::loopEvent()
{
	finishedEvent();
}
