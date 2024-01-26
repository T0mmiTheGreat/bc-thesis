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

#include "constants.hpp"

SDLManager::SDLManager()
	: sdl(SDL_INIT_VIDEO)
	, sdlTtf()
	, window("Caption", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 480, 360, SDL_WINDOW_FULLSCREEN_DESKTOP)
	, renderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)
	, m_eventLoopstate{EVENTLOOP_PRERUN}
	, m_fonts{
		//           font file path     font size
		SDL2pp::Font(FONT_PATH_OLDENGL, 72),
		SDL2pp::Font(FONT_PATH_SEGOEPR, 66),
		SDL2pp::Font(FONT_PATH_BRLNSTR, 20),
		SDL2pp::Font(FONT_PATH_TAHOMA,  20),
	}
	, m_images{
		//              renderer  image file path
		SDL2pp::Texture(renderer, IMAGE_PATH_ICON_NEW),
		SDL2pp::Texture(renderer, IMAGE_PATH_ICON_OPEN),
		SDL2pp::Texture(renderer, IMAGE_PATH_ICON_SAVE),
		SDL2pp::Texture(renderer, IMAGE_PATH_ICON_SAVE_AS),
		SDL2pp::Texture(renderer, IMAGE_PATH_ICON_UNDO),
		SDL2pp::Texture(renderer, IMAGE_PATH_ICON_REDO),
		SDL2pp::Texture(renderer, IMAGE_PATH_ICON_SELECT_TOOL),
		SDL2pp::Texture(renderer, IMAGE_PATH_ICON_PLAYER_TOOL),
		SDL2pp::Texture(renderer, IMAGE_PATH_ICON_OBSTACLE_TOOL),
	}
{
	renderer.SetDrawBlendMode(SDL_BLENDMODE_BLEND);
}

void SDLManager::runEventLoop()
{
	// There would be no point in running the event loop if there wasn't
	// a subscriber which subscribes to the events
	assert(m_subscriber != nullptr);

	// Send start event and modify the eventLoopState property accordingly
	this->m_eventLoopstate = EVENTLOOP_PRESTARTEVENT;
	m_subscriber->startEvent();
	this->m_eventLoopstate = EVENTLOOP_RUNNING;

	window.Show();

	// The event loop itself
	SDL_Event ev;
	bool isRunning = true;
	while (isRunning) {
		// Poll all events
		while (SDL_PollEvent(&ev)) {
			m_subscriber->generalEvent(ev);
			if (ev.type == SDL_QUIT) {
				isRunning = false;
				break;
			}
		}
		if (!isRunning) break;

		m_subscriber->loopEvent();

		// Painting
		if (this->needsRepaint()) {
			// Clear
			renderer.SetTarget();
			renderer.SetDrawColor();
			renderer.Clear();

			// Paint
			m_subscriber->paintEvent(this->m_invalidRect);

			// Present
			renderer.Present();

			// The screen is now valid - set invalid rect area to 0
			m_invalidRect.w = 0;
			m_invalidRect.h = 0;
		}
	}

	this->m_eventLoopstate = EVENTLOOP_POSTRUN;
}

void SDLManager::invalidateRect(SDL_Rect& rect)
{
	SDL_UnionRect(&m_invalidRect, &rect, &m_invalidRect);
}

KeyCode SDLManager::sdlKeycodeToEnum(SDL_Keycode sdlk)
{
	switch (sdlk) {
		case SDLK_RETURN: return KEY_RETURN;
		case SDLK_LEFT:   return KEY_LEFT_ARROW;
		case SDLK_UP:     return KEY_UP_ARROW;
		case SDLK_RIGHT:  return KEY_RIGHT_ARROW;
		case SDLK_DOWN:   return KEY_DOWN_ARROW;
		case SDLK_a:      return KEY_A;
		case SDLK_w:      return KEY_W;
		case SDLK_d:      return KEY_D;
		case SDLK_s:      return KEY_S;
		case SDLK_h:      return KEY_H;
		case SDLK_u:      return KEY_U;
		case SDLK_k:      return KEY_K;
		case SDLK_j:      return KEY_J;
		default: return KEY_UNKNOWN;
	}
}

MouseBtn SDLManager::sdlMouseBtnToEnum(Uint8 btn)
{
	switch (btn) {
		case SDL_BUTTON_LEFT:   return BTN_LEFT;
		case SDL_BUTTON_MIDDLE: return BTN_MIDDLE;
		case SDL_BUTTON_RIGHT:  return BTN_RIGHT;
		default: return BTN_UNKNOWN;
	}
}

Rect SDLManager::sdlRectToRect(const SDL_Rect& rect)
{
	return Rect(rect.x, rect.y, rect.w, rect.h);
}

Size2d SDLManager::sdlPointToSize(const SDL2pp::Point& point)
{
	return Size2d(point.GetX(), point.GetY());
}

SDL_Scancode SDLManager::enumToSdlScancode(KeyCode keyCode)
{
	switch (keyCode) {
		case KEY_RETURN:      return SDL_SCANCODE_RETURN;
		case KEY_LEFT_ARROW:  return SDL_SCANCODE_LEFT;
		case KEY_UP_ARROW:    return SDL_SCANCODE_UP;
		case KEY_RIGHT_ARROW: return SDL_SCANCODE_RIGHT;
		case KEY_DOWN_ARROW:  return SDL_SCANCODE_DOWN;
		case KEY_A:           return SDL_SCANCODE_A;
		case KEY_D:           return SDL_SCANCODE_D;
		case KEY_H:           return SDL_SCANCODE_H;
		case KEY_J:           return SDL_SCANCODE_J;
		case KEY_K:           return SDL_SCANCODE_K;
		case KEY_S:           return SDL_SCANCODE_S;
		case KEY_U:           return SDL_SCANCODE_U;
		case KEY_W:           return SDL_SCANCODE_W;
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

SDL2pp::Color SDLManager::colorToSdlColor(const Color& color)
{
	return SDL2pp::Color(color.r, color.g, color.b, color.a);
}

