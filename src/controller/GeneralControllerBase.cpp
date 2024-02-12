/**
 * @file GeneralControllerBase.cpp
 * @author Tomáš Ludrovan
 * @brief GeneralControllerBase class
 * @version 0.1
 * @date 2023-12-18
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "controller/GeneralControllerBase.hpp"

std::shared_ptr<IControllerChild> GeneralControllerBase::createReplacement()
{
	return nullptr;
}

GeneralControllerBase::GeneralControllerBase(
	std::shared_ptr<ISysProxy> sysProxy)
	: parent{nullptr}
	, sysProxy{sysProxy}
{}

void GeneralControllerBase::start()
{
	startedEvent();
	activatedEvent();
}

void GeneralControllerBase::finish()
{
	deactivatedEvent();
	finishedEvent();
}

void GeneralControllerBase::stop()
{
	deactivatedEvent();
	stoppedEvent();
	finishedEvent();
}

void GeneralControllerBase::abort()
{
	deactivatedEvent();
	abortedEvent();
	finishedEvent();
}

void GeneralControllerBase::pause()
{
	deactivatedEvent();
	pausedEvent();
}

void GeneralControllerBase::resume()
{
	resumedEvent();
	activatedEvent();
}

void GeneralControllerBase::activatedEvent()
{
	sysProxy->invalidateRect();
}

void GeneralControllerBase::deactivatedEvent()
{
	// Ignore
}

void GeneralControllerBase::startedEvent()
{
	// Ignore
}

void GeneralControllerBase::finishedEvent()
{
	if (parent != nullptr) {
		auto replacement = createReplacement();
		parent->replaceController(std::move(replacement));
	}
}

void GeneralControllerBase::pausedEvent()
{
	if (parent != nullptr) {
		auto replacement = createReplacement();
		parent->pauseController(std::move(replacement));
	}
}

void GeneralControllerBase::resumedEvent()
{
	// Ignore
}

void GeneralControllerBase::stoppedEvent()
{
	// Ignore
}

void GeneralControllerBase::abortedEvent()
{
	// Ignore
}

void GeneralControllerBase::keyDownEvent(KeyCode key)
{
	(void)key;
	// Ignore
}

void GeneralControllerBase::textInputEvent(const char* text)
{
	(void)text;
	// Ignore
}

void GeneralControllerBase::mouseBtnDownEvent(MouseBtn btn, int x, int y)
{
	(void)btn;
	(void)x;
	(void)y;
	// Ignore
}

void GeneralControllerBase::mouseBtnUpEvent(MouseBtn btn, int x, int y)
{
	(void)btn;
	(void)x;
	(void)y;
	// Ignore
}

void GeneralControllerBase::mouseMoveEvent(int x, int y)
{
	(void)x;
	(void)y;
	// Ignore
}

void GeneralControllerBase::mouseWheelEvent(int dx, int dy)
{
	(void)dx;
	(void)dy;
	// Ignore
}

void GeneralControllerBase::loopEvent()
{
	// Ignore
}

void GeneralControllerBase::paintEvent(std::shared_ptr<ICanvas> canvas,
	const Rect& invalidRect)
{
	(void)canvas;
	(void)invalidRect;
	// Ignore
}

void GeneralControllerBase::setParent(IControllerChild::IParent* parent)
{
	this->parent = parent;
}
