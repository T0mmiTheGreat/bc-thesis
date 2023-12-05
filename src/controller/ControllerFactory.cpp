/**
 * @file ControllerFactory.cpp
 * @author Tomáš Ludrovan
 * @brief ControllerFactory class
 * @version 0.1
 * @date 2023-12-03
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "controller/ControllerFactory.hpp"

#include "controller/RootController.hpp"
#include "controller/InitialController.hpp"
#include "controller/LogoController.hpp"
#include "controller/InGameController.hpp"

std::shared_ptr<IController> ControllerFactory::createRootController()
{
	return std::make_shared<RootController>();
}

std::unique_ptr<IController> ControllerFactory::createInitialController()
{
	return std::make_unique<InitialController>();
}

std::unique_ptr<IController> ControllerFactory::createLogoController()
{
	return std::make_unique<LogoController>();
}

std::unique_ptr<IController> ControllerFactory::createInGameController()
{
	return std::make_unique<InGameController>();
}
