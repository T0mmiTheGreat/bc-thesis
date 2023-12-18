/**
 * @file IRootController.hpp
 * @author Tomáš Ludrovan
 * @brief IRootController interface
 * @version 0.1
 * @date 2023-12-18
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef IROOTCONTROLLER_HPP
#define IROOTCONTROLLER_HPP

#include "syssubscriber/ISysSubscriber.hpp"

class IRootController : public ISysSubscriber {
public:
	virtual ~IRootController() {}
	/**
	 * @brief Runs the controller and waits until it finishes.
	 */
	virtual void run() = 0;
};

#endif // IROOTCONTROLLER_HPP
