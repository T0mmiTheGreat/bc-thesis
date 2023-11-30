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
#include "SysEventPublisherBase.hpp"

template<typename... Args>
inline void SysEventPublisherBase::notifySubscriber(void(ISysEventSubscriber::* method)(Args...), Args... args)
{
	if (auto subscriberLock = this->subscriber.lock()) {
		((*subscriberLock).*method)(args...);
	}
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

void SysEventPublisherBase::startEvent()
{
	notifySubscriber(&ISysEventSubscriber::startEvent);
}

void SysEventPublisherBase::quitEvent()
{
	notifySubscriber(&ISysEventSubscriber::quitEvent);
}

void SysEventPublisherBase::runPublisher(std::weak_ptr<ISysEventSubscriber> subscriber)
{
	this->subscriber = subscriber;
	startEvent();
}
