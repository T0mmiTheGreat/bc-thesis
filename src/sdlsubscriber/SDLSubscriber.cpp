/**
 * @file SDLSubscriber.cpp
 * @author Tomáš Ludrovan
 * @brief SDLSubscriber class
 * @version 0.1
 * @date 2023-12-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "sdlsubscriber/SDLSubscriber.hpp"
#include "sdlmanager/SDLManager.hpp"
#include "canvas/CanvasFactory.hpp"
#include "SDLSubscriber.hpp"

void SDLSubscriber::generalEvent(const SDL_Event& ev)
{
	if (auto lockedSubscriber = m_subscriber.lock()) {
		switch (ev.type) {
			case SDL_KEYDOWN:
				lockedSubscriber->keyDownEvent(SDLManager::sdlKeycodeToEnum(ev.key.keysym.sym));
				break;
			case SDL_TEXTINPUT:
				lockedSubscriber->textInputEvent(ev.text.text);
				break;
			case SDL_MOUSEBUTTONDOWN:
				lockedSubscriber->mouseBtnDownEvent(SDLManager::sdlMouseBtnToEnum(ev.button.button), ev.button.x, ev.button.y);
				break;
			case SDL_MOUSEBUTTONUP:
				lockedSubscriber->mouseBtnUpEvent(SDLManager::sdlMouseBtnToEnum(ev.button.button), ev.button.x, ev.button.y);
				break;
			case SDL_MOUSEMOTION:
				lockedSubscriber->mouseMoveEvent(ev.motion.x, ev.motion.y);
				break;
			case SDL_MOUSEWHEEL:
				lockedSubscriber->mouseWheelEvent(ev.wheel.x, ev.wheel.y);
				break;
			case SDL_QUIT:
				lockedSubscriber->quitEvent();
				break;
		}
	}
}

void SDLSubscriber::paintEvent(const SDL_Rect& invalidRect)
{
	if (auto lockedSubscriber = m_subscriber.lock()) {
		// Send event
		Rect r = SDLManager::sdlRectToRect(invalidRect);
		lockedSubscriber->paintEvent(CanvasFactory::createSdlCanvas(), r);
	}
}

void SDLSubscriber::loopEvent()
{
	if (auto lockedSubscriber = m_subscriber.lock()) {
		lockedSubscriber->loopEvent();
	}
}
void SDLSubscriber::startEvent()
{
	if (auto lockedSubscriber = m_subscriber.lock()) {
		lockedSubscriber->startEvent();
	}
}

