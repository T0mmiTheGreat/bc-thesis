/**
 * @file ControllerFactory.cpp
 * @author Tomáš Ludrovan
 * @brief ControllerFactory class
 * @version 0.1
 * @date 2023-12-18
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "controller/ControllerFactory.hpp"

#include <memory>

#include "controller/RootController.hpp"
#include "controller/InitialController.hpp"
#include "controller/LogoController.hpp"

std::shared_ptr<IRootController> ControllerFactory::createRootController()
{
	return std::make_shared<RootController>();
}

std::unique_ptr<IControllerChild> ControllerFactory::createInitialController()
{
	return std::make_unique<InitialController>();
}

std::unique_ptr<IControllerChild> ControllerFactory::createLogoController()
{
	return std::make_unique<LogoController>();
}
