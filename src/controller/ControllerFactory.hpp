/**
 * @file ControllerFactory.hpp
 * @author Tomáš Ludrovan
 * @brief ControllerFactory class
 * @version 0.1
 * @date 2023-12-03
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef CONTROLLERFACTORY_HPP
#define CONTROLLERFACTORY_HPP

#include <memory>

#include "controller/IController.hpp"

/**
 * @brief Factory for the IController interface.
 */
class ControllerFactory {
public:
	static std::shared_ptr<IController> createRootController();
	static std::unique_ptr<IController> createInitialController();
	static std::unique_ptr<IController> createLogoController();
	static std::unique_ptr<IController> createInGameController();
};

#endif // CONTROLLERFACTORY_HPP
