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

#include "ISysEventPublisher.hpp"
#include "ISysEventSubscriber.hpp"

class SysEventPublisherBase : public ISysEventPublisher {
private:
	template <typename... Args>
	void notifySubscriber(void (ISysEventSubscriber::*method)(Args...), Args... args);
protected:
	std::weak_ptr<ISysEventSubscriber> subscriber;

	void keyDownEvent(KeyCode key);
	void mouseBtnDownEvent(MouseBtn btn);
	void mouseMoveEvent(int x, int y);
	void quitEvent();
public:
	virtual void run(std::weak_ptr<ISysEventSubscriber> subscriber) override;
};

#endif // SYSEVENTPUBLISHERBASE_HPP
