/**
 * @file SDLSysProxy.cpp
 * @author Tomáš Ludrovan
 * @brief SDLSysProxy class
 * @version 0.1
 * @date 2023-12-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "sysproxy/SDLSysProxy.hpp"

#include <SDL2/SDL.h>

#include "sdlmanager/SDLManager.hpp"

void SDLSysProxy::invalidateRect(const Rect& rect)
{
	SDL_Rect r = SDLManager::rectToSdlRect(rect);
	SDLManager::get().invalidateRect(r);
}

bool SDLSysProxy::isKeyPressed(KeyCode keyCode)
{
	return SDL_GetKeyboardState(NULL)[SDLManager::enumToSdlScancode(keyCode)];
}
