/**
 * @file RootController.cpp
 * @author Tomáš Ludrovan
 * @brief RootController class
 * @version 0.1
 * @date 2023-12-18
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "controller/RootController.hpp"

#include "controller/ControllerFactory.hpp"

RootController::RootController(std::shared_ptr<ISysProxy> sysProxy)
	: ControllerBase()
	, m_sysProxy{sysProxy}
{
	setChildController(ControllerFactory::createInitialController(sysProxy));
}

void RootController::startEvent()
{
	// Propagate (as different event)
	// Looks the same but have different semantics
	m_childController->startedEvent();
}

void RootController::quitEvent()
{
	// Propagate (as different event)
	m_childController->stoppedEvent();
}

void RootController::keyDownEvent(KeyCode key)
{
	// Propagate
	m_childController->keyDownEvent(key);
}

void RootController::mouseBtnDownEvent(MouseBtn btn, int x, int y)
{
	// Propagate
	m_childController->mouseBtnDownEvent(btn, x, y);
}

void RootController::mouseBtnUpEvent(MouseBtn btn, int x, int y)
{
	// Propagate
	m_childController->mouseBtnUpEvent(btn, x, y);
}

void RootController::mouseMoveEvent(int x, int y)
{
	// Propagate
	m_childController->mouseMoveEvent(x, y);
}

void RootController::mouseWheelEvent(int dx, int dy)
{
	// Propagate
	m_childController->mouseWheelEvent(dx, dy);
}

void RootController::loopEvent()
{
	// Propagate
	m_childController->loopEvent();
}

void RootController::paintEvent(std::shared_ptr<ICanvas> canvas, const Rect& invalidRect)
{
	// Propagate
	m_childController->paintEvent(canvas, invalidRect);
}

void RootController::run()
{
	m_sysProxy->runEventLoop();
}

void RootController::setChildController(std::unique_ptr<IControllerChild> value)
{
	if (value != nullptr) {
		// Set child controller
		m_childController = std::move(value);
		// Assign swap callback to the controller
		m_childController->setSwapCallback([this](std::unique_ptr<IControllerChild> newValue) {
			this->setChildController(std::move(newValue));
		});
		// If the event loop is already running, send the started event straight
		// away. If it's not running yet, it will be sent once the event loop
		// starts
		if (m_sysProxy->getEventLoopState() == EVENTLOOP_RUNNING) {
			m_childController->startedEvent();
		}
	}
	else {
		m_childController = nullptr;
	}
}
