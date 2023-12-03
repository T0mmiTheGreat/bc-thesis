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

class IControllerEventSubscriber : public ISysSubscriber {
public:
	virtual void finishedEvent() = 0;
	virtual void abortEvent() = 0;
	// TODO: more events
	// XXX: add them to ControllerBase too
};

#endif // ICONTROLLEREVENTSUBSCRIBER_HPP
