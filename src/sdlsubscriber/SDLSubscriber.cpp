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

#include "SDLSubscriber.hpp"

KeyCode SDLSubscriber::sdlKeycodeToEnum(SDL_Keycode sdlk)
{
	switch (sdlk) {
		case SDLK_RETURN: return KEY_RETURN;
		case SDLK_LEFT: return KEY_LEFT_ARROW;
		case SDLK_UP: return KEY_UP_ARROW;
		case SDLK_RIGHT: return KEY_RIGHT_ARROW;
		case SDLK_DOWN: return KEY_DOWN_ARROW;
		// TODO: more keys
		default: return KEY_UNKNOWN;
	}
}

MouseBtn SDLSubscriber::sdlMouseBtnToEnum(Uint8 btn)
{
	switch (btn) {
		case SDL_BUTTON_LEFT: return BTN_LEFT;
		case SDL_BUTTON_MIDDLE: return BTN_MIDDLE;
		case SDL_BUTTON_RIGHT: return BTN_RIGHT;
		default: return BTN_UNKNOWN;
	}
}

void SDLSubscriber::generalEvent(SDL_Event& ev)
{
	if (auto lockedSubscriber = m_subscriber.lock()) {
		switch (ev.type) {
			case SDL_KEYDOWN:
				lockedSubscriber->keyDownEvent(sdlKeycodeToEnum(ev.key.keysym.sym));
				break;
			case SDL_MOUSEBUTTONDOWN:
				lockedSubscriber->mouseBtnDownEvent(sdlMouseBtnToEnum(ev.button.button));
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
