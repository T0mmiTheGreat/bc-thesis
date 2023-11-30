/**
 * @file ISysEventPublisher.hpp
 * @author Tomáš Ludrovan
 * @brief ISysEventPublisher interface
 * @version 0.1
 * @date 2023-11-29
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef ISYSEVENTPUBLISHER_HPP
#define ISYSEVENTPUBLISHER_HPP

#include <memory>

#include "sysevent/ISysEventSubscriber.hpp"

/**
 * @brief Interface for simple system even publisher.
 * 
 * @details The publisher runs until quit event is published. After that, the
 *          run() may not be executed again, not even with another instance.
 */
class ISysEventPublisher {
public:
	virtual ~ISysEventPublisher() {}
	/**
	 * @brief Runs the event loop.
	 * 
	 * @details Runs synchronusly and ends when the application closes (QUIT
	 *          event is received).
	 * 
	 *          Upon calling it immediatelly sends startEvent to the subscriber.
	 * 
	 *          This method may be executed at most once across all instances
	 *          of ISysEventPublisher.
	 * 
	 * @param subscriber The object that will subscribe the events published
	 *                   during the event loop.
	 */
	virtual void run(std::weak_ptr<ISysEventSubscriber> subscriber) = 0;
};

#endif // ISYSEVENTPUBLISHER_HPP
