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

std::shared_ptr<IControllerChild> InitialController::createReplacement()
{
	//return ControllerFactory::createInGameController(sysProxy);
	//return ControllerFactory::createLogoController(sysProxy);
	return ControllerFactory::createStageEditorController(sysProxy);
	//return ControllerFactory::createMainMenuController(sysProxy);
}

void InitialController::loopEvent()
{
	finish();
}
