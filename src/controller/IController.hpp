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

/**
 * @brief Controller interface.
 * 
 * @details Controllers create the flow of the application. Each controller
 *          represents a piece of the application logic ranging from the whole
 *          screens and their layouts to individual screen components, like
 *          buttons or labels.
 * 
 *          There is one main RootController which listens to events from
 *          system (system events, meaning SDL events, for example) and sends
 *          them to its child. The child is the top-level controller which has
 *          the most control over the application. Once the controller finishes
 *          it should return its successor which becomes the new child of the
 *          RootController.
 * 
 *          Other controllers may have similar functionality to the Root. Some
 *          may have a number of children and may send them events from their
 *          parent. This means that they may form a "controller tree". These
 *          trees won't be very deep and they will always have only one branch
 *          in the root.
 * 
 *          The controller run loop (controllers activating and deactivating)
 *          runs in a thread which is separate from the event loop.
 */
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
