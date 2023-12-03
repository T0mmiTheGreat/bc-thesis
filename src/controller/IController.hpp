/**
 * @file IController.hpp
 * @author Tomáš Ludrovan
 * @brief IController interface
 * @version 0.1
 * @date 2023-11-29
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef ICONTROLLER_HPP
#define ICONTROLLER_HPP

#include <memory>

#include "controller/IControllerEventSubscriber.hpp"

class IController : public IControllerEventSubscriber {
public:
	virtual ~IController() {}
	/**
	 * @brief Activates a controller and waits until it deactivates itself.
	 * 
	 * @return The controller that should replace it when it deactivates, or
	 *         nullptr if there is no replacement.
	 * 
	 * @remark The caller might check (using assert()) whether the method did
	 *         or didn't return a replacement controller as expected.
	 */
	virtual std::unique_ptr<IController> runController() = 0;
};

#endif // ICONTROLLER_HPP
