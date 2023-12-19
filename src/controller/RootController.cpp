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
#include "sysproxy/SysProxyFactory.hpp"

RootController::RootController()
	: ControllerBase()
{
	setChildController(ControllerFactory::createInitialController());
}

void RootController::startEvent()
{
	m_childController->startedEvent();
}

void RootController::quitEvent()
{
	m_childController->stoppedEvent();
}

void RootController::keyDownEvent(KeyCode key)
{
	m_childController->keyDownEvent(key);
}

void RootController::mouseBtnDownEvent(MouseBtn btn)
{
	m_childController->mouseBtnDownEvent(btn);
}

void RootController::mouseMoveEvent(int x, int y)
{
	m_childController->mouseMoveEvent(x, y);
}

void RootController::frameEvent()
{
	m_childController->frameEvent();
}

void RootController::paintEvent(std::shared_ptr<ICanvas> canvas, Rect& invalidRect)
{
	m_childController->paintEvent(canvas, invalidRect);
}

void RootController::run()
{
	SysProxyFactory::createDefault()->runEventLoop();
}

void RootController::setChildController(std::unique_ptr<IControllerChild> value)
{
	if (value != nullptr) {
		m_childController = std::move(value);
		m_childController->setSwapCallback([this](std::unique_ptr<IControllerChild> newValue) {
			this->setChildController(std::move(newValue));
		});
		if (SysProxyFactory::createDefault()->getEventLoopState() == EVENTLOOP_RUNNING) {
			m_childController->startedEvent();
		}
	}
	else {
		m_childController = nullptr;
	}
}
