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
#include "controller/ControllerFactory.hpp"

std::unique_ptr<IController> InitialController::runController()
{
	GeneralControllerBase::runController();
	return ControllerFactory::createInGameController();
}

void InitialController::startedEvent()
{
	finishedEvent();
}
