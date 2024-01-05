/**
 * @file ControllerFactory.hpp
 * @author Tomáš Ludrovan
 * @brief ControllerFactory class
 * @version 0.1
 * @date 2023-12-18
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef CONTROLLERFACTORY_HPP
#define CONTROLLERFACTORY_HPP

#include <memory>

#include "controller/IRootController.hpp"
#include "controller/IControllerChild.hpp"
#include "sysproxy/ISysProxy.hpp"

/**
 * @brief Factory for the IRootController and IControllerChild interfaces.
 */
class ControllerFactory {
public:
	static std::shared_ptr<IRootController> createRootController(std::shared_ptr<ISysProxy> sysProxy);
	static std::unique_ptr<IControllerChild> createInitialController(std::shared_ptr<ISysProxy> sysProxy);
	static std::unique_ptr<IControllerChild> createLogoController(std::shared_ptr<ISysProxy> sysProxy);
	static std::unique_ptr<IControllerChild> createMainMenuController(std::shared_ptr<ISysProxy> sysProxy);
	static std::unique_ptr<IControllerChild> createInGameController(std::shared_ptr<ISysProxy> sysProxy);
};

#endif // CONTROLLERFACTORY_HPP
