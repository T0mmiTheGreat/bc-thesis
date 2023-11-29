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
#ifndef ISYSEVENTPUBLISHERLISTENER_HPP
#define ISYSEVENTPUBLISHERLISTENER_HPP

#include <memory>

#include "ISysEventSubscriber.hpp"

/**
 * @brief Interface for simple system even publisher.
 * 
 * @details The publisher runs until quit event is published. After that, the
 *          run() may not be executed again, not even with another instance.
 */
class ISysEventPublisher {
public:
	virtual ~ISysEventPublisher() {}
	virtual void run(std::weak_ptr<ISysEventSubscriber> subscriber) = 0;
};

#endif // ISYSEVENTPUBLISHERLISTENER_HPP
