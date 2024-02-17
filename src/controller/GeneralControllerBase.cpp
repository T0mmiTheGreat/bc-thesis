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

void GeneralControllerBase::replaceController()
{
	if (parent != nullptr) {
		auto replacement = createReplacement();
		parent->replaceController(std::move(replacement));
	}
}

void GeneralControllerBase::pauseController()
{
	if (parent != nullptr) {
		auto replacement = createReplacement();
		parent->pauseController(std::move(replacement));
	}
}

std::shared_ptr<IControllerChild> GeneralControllerBase::createReplacement()
{
	return nullptr;
}

void GeneralControllerBase::onActivated()
{
	sysProxy->invalidateRect();
}

void GeneralControllerBase::onDeactivated()
{
	// Ignore
}

void GeneralControllerBase::onStarted()
{
	// Ignore
}

void GeneralControllerBase::onFinished()
{
	// Ignore
}

void GeneralControllerBase::onStopped()
{
	// Ignore
}

void GeneralControllerBase::onAborted()
{
	// Ignore
}

void GeneralControllerBase::onPaused()
{
	// Ignore
}

void GeneralControllerBase::onResumed()
{
	// Ignore
}

void GeneralControllerBase::onKeyDown(KeyCode key)
{
	(void)key;
	// Ignore
}

void GeneralControllerBase::onTextInput(const char* text)
{
	(void)text;
	// Ignore
}

void GeneralControllerBase::onMouseBtnDown(MouseBtn btn, int x, int y)
{
	(void)btn;
	(void)x;
	(void)y;
	// Ignore
}

void GeneralControllerBase::onMouseBtnUp(MouseBtn btn, int x, int y)
{
	(void)btn;
	(void)x;
	(void)y;
	// Ignore
}

void GeneralControllerBase::onMouseMove(int x, int y)
{
	(void)x;
	(void)y;
	// Ignore
}

void GeneralControllerBase::onMouseWheel(int dx, int dy)
{
	(void)dx;
	(void)dy;
	// Ignore
}

void GeneralControllerBase::onLoop()
{
	// Ignore
}

void GeneralControllerBase::onPaint(std::shared_ptr<ICanvas> canvas,
	const Rect& invalidRect)
{
	(void)canvas;
	(void)invalidRect;
	// Ignore
}

GeneralControllerBase::GeneralControllerBase(
	std::shared_ptr<ISysProxy> sysProxy)
	: parent{nullptr}
	, sysProxy{sysProxy}
{}

void GeneralControllerBase::startedEvent()
{
	onStarted();
	onActivated();
}

void GeneralControllerBase::finishedEvent()
{
	onDeactivated();
	onFinished();
	replaceController();
}

void GeneralControllerBase::pausedEvent()
{
	onDeactivated();
	onPaused();
	pauseController();
}

void GeneralControllerBase::resumedEvent()
{
	onResumed();
	onActivated();
}

void GeneralControllerBase::stoppedEvent()
{
	onDeactivated();
	onStopped();
	replaceController();
}

void GeneralControllerBase::abortedEvent()
{
	onDeactivated();
	onAborted();
	replaceController();
}

void GeneralControllerBase::keyDownEvent(KeyCode key)
{
	onKeyDown(key);
}

void GeneralControllerBase::textInputEvent(const char* text)
{
	onTextInput(text);
}

void GeneralControllerBase::mouseBtnDownEvent(MouseBtn btn, int x, int y)
{
	onMouseBtnDown(btn, x, y);
}

void GeneralControllerBase::mouseBtnUpEvent(MouseBtn btn, int x, int y)
{
	onMouseBtnUp(btn, x, y);
}

void GeneralControllerBase::mouseMoveEvent(int x, int y)
{
	onMouseMove(x, y);
}

void GeneralControllerBase::mouseWheelEvent(int dx, int dy)
{
	onMouseWheel(dx, dy);
}

void GeneralControllerBase::loopEvent()
{
	onLoop();
}

void GeneralControllerBase::paintEvent(std::shared_ptr<ICanvas> canvas,
	const Rect& invalidRect)
{
	onPaint(canvas, invalidRect);
}

void GeneralControllerBase::setParent(IControllerChild::IParent* parent)
{
	this->parent = parent;
}
