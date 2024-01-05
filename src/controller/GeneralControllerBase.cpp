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
}

void GeneralControllerBase::mouseBtnDownEvent(MouseBtn btn)
{
}

void GeneralControllerBase::mouseMoveEvent(int x, int y)
{
}

void GeneralControllerBase::loopEvent()
{
}

void GeneralControllerBase::paintEvent(std::shared_ptr<ICanvas> canvas, Rect & invalidRect)
{
}

void GeneralControllerBase::setSwapCallback(SwapCallback f)
{
	swapCallback = f;
}
