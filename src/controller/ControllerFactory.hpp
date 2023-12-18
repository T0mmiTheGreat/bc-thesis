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

class ControllerFactory {
public:
	static std::shared_ptr<IRootController> createRootController();
	static std::unique_ptr<IControllerChild> createInitialController();
	static std::unique_ptr<IControllerChild> createLogoController();
};

#endif // CONTROLLERFACTORY_HPP
