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

#include "types.hpp"

void GeneralControllerBase::sendStartedEventIfEventLoopRunning()
{
	if (sysProxy->getEventLoopState() == EVENTLOOP_RUNNING) {
		startedEvent();
	}
}

std::unique_ptr<IController> GeneralControllerBase::runController()
{
	sendStartedEventIfEventLoopRunning();

	// Acquire lock for the isControllerFinished variable
	std::unique_lock<std::mutex> lk(mutexIsControllerFinished);

	// Unlock the variable, wait until it's `true`
	cvIsControllerFinished.wait(lk, [this]() { return this->isControllerFinished; });

	return nullptr;

	// Release the lock for the isControllerFinished variable
}

void GeneralControllerBase::startEvent()
{
	startedEvent();
}

void GeneralControllerBase::quitEvent()
{
	abortEvent();
}

void GeneralControllerBase::finishedEvent()
{
	{
		// Acquire lock for the isControllerFinished variable
		std::lock_guard<std::mutex> lk(mutexIsControllerFinished);

		// Modify its value
		isControllerFinished = true;
	}
	// Release the lock for the isControllerFinished variable

	// Notify `runController()` that the value has changed
	cvIsControllerFinished.notify_one();
}

void GeneralControllerBase::abortEvent()
{
	finishedEvent();
}
