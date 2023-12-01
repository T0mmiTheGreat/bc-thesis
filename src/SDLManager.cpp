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

#include "SDLManager.hpp"

#include <SDL2/SDL.h>

bool SDLManager::needsRepaint() const
{
	return !SDL_RectEmpty(&m_invalidRect);
}

void SDLManager::runEventLoop(void (*eventCapturer)(SDL_Event& ev), void (*paintEventCapturer)(SDL_Rect& invalidRect))
{
	SDL_Event ev;
	bool isRunning = true;
	while (isRunning) {
		while (SDL_PollEvent(&ev)) {
			eventCapturer(ev);
			if (this->needsRepaint()) {
				paintEventCapturer(this->m_invalidRect);
			}
		}
	}
}

void SDLManager::invalidateRect(SDL_Rect& rect)
{
	SDL_UnionRect(&m_invalidRect, &rect, &m_invalidRect);
}
