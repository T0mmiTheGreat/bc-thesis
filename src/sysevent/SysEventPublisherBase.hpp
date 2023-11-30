/**
 * @file SysEventPublisherBase.hpp
 * @author Tomáš Ludrovan
 * @brief SysEventPublisherBase abstract class
 * @version 0.1
 * @date 2023-11-29
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef SYSEVENTPUBLISHERBASE_HPP
#define SYSEVENTPUBLISHERBASE_HPP

#include <memory>
#include <set>
#include <thread>

#include "sysevent/ISysEventPublisher.hpp"
#include "sysevent/ISysEventSubscriber.hpp"

/**
 * @brief Base class for all Sys Event Publishers.
 * 
 * @details Provides convenient implementations of methods for the descendants.
 *          Typically, the concrete implementation will "switch" the event
 *          received from system and call the corresponding method defined by
 *          this class.
 */
class SysEventPublisherBase : public ISysEventPublisher {
private:
	/**
	 * @brief Generic method for passing an event to the subscriber.
	 * 
	 * @tparam Args The types of arguments passed to the subscriber method.
	 * @param method The subscriber method to call.
	 * @param args Arguments to pass to the method.
	 */
	template <typename... Args>
	void notifySubscriber(void (ISysEventSubscriber::*method)(Args...), Args... args);
protected:
	/**
	 * @brief Object that subscribes to the events.
	 * 
	 */
	std::weak_ptr<ISysEventSubscriber> subscriber;

	void keyDownEvent(KeyCode key);
	void mouseBtnDownEvent(MouseBtn btn);
	void mouseMoveEvent(int x, int y);
	void quitEvent();
public:
	virtual void run(std::weak_ptr<ISysEventSubscriber> subscriber) override;
};

#endif // SYSEVENTPUBLISHERBASE_HPP
