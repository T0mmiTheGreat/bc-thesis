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

void SDLManager::runEventLoop()
{
	assert(m_subscriber != nullptr);

	this->m_state = MSTATE_RUNNING;

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
			m_subscriber->paintEvent(this->m_invalidRect);
		}
	}
}

void SDLManager::invalidateRect(SDL_Rect& rect)
{
	SDL_UnionRect(&m_invalidRect, &rect, &m_invalidRect);
}
