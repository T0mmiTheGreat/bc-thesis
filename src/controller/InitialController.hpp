/**
 * @file InitialController.hpp
 * @author Tomáš Ludrovan
 * @brief InitialController class
 * @version 0.1
 * @date 2023-12-03
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef INITIALCONTROLLER_HPP
#define INITIALCONTROLLER_HPP

#include <memory>

#include "controller/GeneralControllerBase.hpp"

/**
 * @brief The first child controller of the Root.
 * 
 * @details Its purpose is to merely choose its successor and don't do anything
 *          else. This is done to follow the Single Responsibility Principle of
 *          SOLID.
 */
class InitialController : public GeneralControllerBase {
public:
	std::unique_ptr<IController> runController() override;

	void startedEvent() override;
};

#endif // INITIALCONTROLLER_HPP
