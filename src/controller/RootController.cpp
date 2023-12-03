/**
 * @file RootController.cpp
 * @author Tomáš Ludrovan
 * @brief RootController class
 * @version 0.1
 * @date 2023-12-03
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "controller/RootController.hpp"

#include <cassert>
#include <thread>

#include "sysproxy/SysProxyFactory.hpp"

void RootController::runChildren()
{
	while (!m_isQuit) {
		auto replacementController = m_childController->runController();
		assert(m_isQuit || replacementController != nullptr);
		m_childController = std::move(replacementController);
	}
}

std::unique_ptr<IController> RootController::runController()
{
	// ControllerBase::runController();  -- Would it work for PVF?
	std::thread childrenLoop(&RootController::runChildren, this);
	SysProxyFactory::createDefault()->runEventLoop();
	childrenLoop.join();
	return nullptr;
}

void RootController::startEvent()
{
	m_childController->startEvent();
}

void RootController::quitEvent()
{
	// The order is important. First set the quit flag, then send the event to
	// the child. When the child deactivates, the quit flag must already be set
	m_isQuit = true;
	m_childController->quitEvent();
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

void RootController::finishedEvent()
{
	m_childController->finishedEvent();
}

void RootController::abortEvent()
{
	m_childController->abortEvent();
}

void RootController::frameEvent()
{
	m_childController->frameEvent();
}

void RootController::paintEvent(std::shared_ptr<ICanvas> canvas, Rect& invalidRect)
{
	m_childController->paintEvent(canvas, invalidRect);
}
