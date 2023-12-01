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

void SDLSubscriber::generalEvent(SDL_Event& ev)
{
	if (auto lockedSubscriber = m_subscriber.lock()) {
		switch (ev.type) {
			case SDL_KEYDOWN:
				lockedSubscriber->keyDownEvent(SDLManager::sdlKeycodeToEnum(ev.key.keysym.sym));
				break;
			case SDL_MOUSEBUTTONDOWN:
				lockedSubscriber->mouseBtnDownEvent(SDLManager::sdlMouseBtnToEnum(ev.button.button));
				break;
			case SDL_MOUSEMOTION:
				lockedSubscriber->mouseMoveEvent(ev.motion.x, ev.motion.y);
				break;
			case SDL_QUIT:
				lockedSubscriber->quitEvent();
				break;
		}
	}
}

void SDLSubscriber::paintEvent(SDL_Rect& invalidRect)
{
	if (auto lockedSubscriber = m_subscriber.lock()) {
		// TODO
	}
}

void SDLSubscriber::frameEvent()
{
	if (auto lockedSubscriber = m_subscriber.lock()) {
		lockedSubscriber->frameEvent();
	}
}
