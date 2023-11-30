/**
 * @file SysEventPublisherBase.cpp
 * @author Tomáš Ludrovan
 * @brief SysEventPublisherBase abstract class
 * @version 0.1
 * @date 2023-11-29
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "sysevent/SysEventPublisherBase.hpp"
#include "sysevent/ISysEventSubscriber.hpp"

template<typename... Args>
inline void SysEventPublisherBase::notifySubscriber(void(ISysEventSubscriber::* method)(Args...), Args... args)
{
	auto subscriberLock = this->subscriber.lock();
	((*subscriberLock).*method)(args...);
}

void SysEventPublisherBase::keyDownEvent(KeyCode key)
{
	notifySubscriber(&ISysEventSubscriber::keyDownEvent, key);
}

void SysEventPublisherBase::mouseBtnDownEvent(MouseBtn btn)
{
	notifySubscriber(&ISysEventSubscriber::mouseBtnDownEvent, btn);
}

void SysEventPublisherBase::mouseMoveEvent(int x, int y)
{
	notifySubscriber(&ISysEventSubscriber::mouseMoveEvent, x, y);
}

void SysEventPublisherBase::quitEvent()
{
	notifySubscriber(&ISysEventSubscriber::quitEvent);
}

void SysEventPublisherBase::run(std::weak_ptr<ISysEventSubscriber> subscriber)
{
	this->subscriber = subscriber;
}
