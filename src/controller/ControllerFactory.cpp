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
#include "controller/MainMenuController.hpp"
#include "controller/InGameController.hpp"
#include "controller/StageEditorController.hpp"

std::shared_ptr<IRootController> ControllerFactory::createRootController(std::shared_ptr<ISysProxy> sysProxy)
{
	return std::make_shared<RootController>(sysProxy);
}

std::unique_ptr<IControllerChild> ControllerFactory::createInitialController(std::shared_ptr<ISysProxy> sysProxy)
{
	return std::make_unique<InitialController>(sysProxy);
}

std::unique_ptr<IControllerChild> ControllerFactory::createLogoController(std::shared_ptr<ISysProxy> sysProxy)
{
	return std::make_unique<LogoController>(sysProxy);
}

std::unique_ptr<IControllerChild> ControllerFactory::createMainMenuController(std::shared_ptr<ISysProxy> sysProxy)
{
	return std::make_unique<MainMenuController>(sysProxy);
}

std::unique_ptr<IControllerChild> ControllerFactory::createInGameController(std::shared_ptr<ISysProxy> sysProxy)
{
	return std::make_unique<InGameController>(sysProxy);
}

std::unique_ptr<IControllerChild> ControllerFactory::createStageEditorController(std::shared_ptr<ISysProxy> sysProxy)
{
	return std::make_unique<StageEditorController>(sysProxy);
}
