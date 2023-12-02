/**
 * @file SDLManager.cpp
 * @author Tomáš Ludrovan
 * @brief SDLManager class
 * @version 0.1
 * @date 2023-12-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "sdlmanager/SDLManager.hpp"

#include <cassert>
#include "SDLManager.hpp"
#include "SDLManager.hpp"

void SDLManager::runEventLoop()
{
	assert(m_subscriber != nullptr);

	this->m_state = MSTATE_RUNNING;

	window.Show();
	SDL_Event ev;
	bool isRunning = true;
	while (isRunning) {
		while (SDL_PollEvent(&ev)) {
			if (ev.type == SDL_QUIT) {
				isRunning = false;
			}
			m_subscriber->generalEvent(ev);
		}
		m_subscriber->frameEvent();
		if (this->needsRepaint()) {
			renderer.SetTarget();
			renderer.SetDrawColor();
			renderer.Clear();
			m_subscriber->paintEvent(this->m_invalidRect);
			renderer.Present();
			m_invalidRect.w = 0;
			m_invalidRect.h = 0;
		}
	}
}

void SDLManager::invalidateRect(SDL_Rect& rect)
{
	SDL_UnionRect(&m_invalidRect, &rect, &m_invalidRect);
}

KeyCode SDLManager::sdlKeycodeToEnum(SDL_Keycode sdlk)
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

MouseBtn SDLManager::sdlMouseBtnToEnum(Uint8 btn)
{
	switch (btn) {
		case SDL_BUTTON_LEFT: return BTN_LEFT;
		case SDL_BUTTON_MIDDLE: return BTN_MIDDLE;
		case SDL_BUTTON_RIGHT: return BTN_RIGHT;
		default: return BTN_UNKNOWN;
	}
}

SDL_Scancode SDLManager::enumToSdlScancode(KeyCode keyCode)
{
	switch (keyCode) {
		case KEY_RETURN: return SDL_SCANCODE_RETURN;
		case KEY_LEFT_ARROW: return SDL_SCANCODE_LEFT;
		case KEY_UP_ARROW: return SDL_SCANCODE_UP;
		case KEY_RIGHT_ARROW: return SDL_SCANCODE_RIGHT;
		case KEY_DOWN_ARROW: return SDL_SCANCODE_DOWN;
		case KEY_UNKNOWN:
		default:
			return SDL_SCANCODE_UNKNOWN;
	}
}

SDL_Rect SDLManager::rectToSdlRect(const Rect& rect)
{
	return SDL_Rect{
		.x = rect.x,
		.y = rect.y,
		.w = rect.w,
		.h = rect.h
	};
}

