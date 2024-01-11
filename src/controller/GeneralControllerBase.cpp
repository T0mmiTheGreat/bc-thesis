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

std::unique_ptr<IControllerChild> GeneralControllerBase::createReplacement()
{
	return nullptr;
}

GeneralControllerBase::GeneralControllerBase(std::shared_ptr<ISysProxy> sysProxy)
	: swapCallback{nullptr}
	, sysProxy{sysProxy}
{}

void GeneralControllerBase::startedEvent()
{
	// Ignore
}

void GeneralControllerBase::finishedEvent()
{
	if (swapCallback != nullptr) {
		swapCallback(createReplacement());
	}
}

void GeneralControllerBase::stoppedEvent()
{
	finishedEvent();
}

void GeneralControllerBase::abortedEvent()
{
	finishedEvent();
}

void GeneralControllerBase::keyDownEvent(KeyCode key)
{
	// Ignore
}

void GeneralControllerBase::mouseBtnDownEvent(MouseBtn btn)
{
	// Ignore
}

void GeneralControllerBase::mouseMoveEvent(int x, int y)
{
	// Ignore
}

void GeneralControllerBase::loopEvent()
{
	// Ignore
}

void GeneralControllerBase::paintEvent(std::shared_ptr<ICanvas> canvas, Rect & invalidRect)
{
	// Ignore
}

void GeneralControllerBase::setSwapCallback(SwapCallback f)
{
	swapCallback = f;
}
