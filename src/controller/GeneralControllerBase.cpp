/**
 * @file GeneralControllerBase.cpp
 * @author Tomáš Ludrovan
 * @brief GeneralControllerBase class
 * @version 0.1
 * @date 2023-12-03
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "controller/GeneralControllerBase.hpp"

#include <mutex>

void GeneralControllerBase::sendStartEventIfEventLoopRunning()
{
	if (sysProxy->isEventLoopRunning()) {
		startEvent();
	}
}

std::unique_ptr<IController> GeneralControllerBase::runController()
{
	sendStartEventIfEventLoopRunning();

	std::unique_lock<std::mutex> lk(mutexIsControllerFinished);
	cvIsControllerFinished.wait(lk, [this]() { return this->isControllerFinished; });
	return nullptr;
}

void GeneralControllerBase::quitEvent()
{
	abortEvent();
}

void GeneralControllerBase::finishedEvent()
{
	{
		std::lock_guard<std::mutex> lk(mutexIsControllerFinished);
		isControllerFinished = true;
	}
	cvIsControllerFinished.notify_one();
}

void GeneralControllerBase::abortEvent()
{
	finishedEvent();
}
