/**
 * @file IControllerEventSubscriber.hpp
 * @author Tomáš Ludrovan
 * @brief IControllerEventSubscriber class
 * @version 0.1
 * @date 2023-11-30
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef ICONTROLLEREVENTSUBSCRIBER_HPP
#define ICONTROLLEREVENTSUBSCRIBER_HPP

#include "syssubscriber/ISysSubscriber.hpp"

/**
 * @brief Defines additional events which controllers send between each other.
 */
class IControllerEventSubscriber : public ISysSubscriber {
public:
	virtual ~IControllerEventSubscriber() {}
	/**
	 * @brief The controller started.
	 * 
	 * @details This happens either when the runController() is called or when
	 *          the event loop starts.
	 */
	virtual void startedEvent() = 0;
	/**
	 * @brief The controller should finish its execution.
	 * 
	 * @details This event usually sends a controller to itself.
	 */
	virtual void finishedEvent() = 0;
	/**
	 * @brief The controller should stop its execution.
	 * 
	 * @details In most scenarios this event may be considered the same as
	 *          finishedEvent. It is used to emphasize that the controller
	 *          finishes prematurely.
	 */
	virtual void abortEvent() = 0;
	// TODO: more events
	// XXX: add them to ControllerBase too
};

#endif // ICONTROLLEREVENTSUBSCRIBER_HPP
