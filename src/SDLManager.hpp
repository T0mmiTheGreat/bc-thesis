/**
 * @file SDLManager.hpp
 * @author Tomáš Ludrovan
 * @brief SDLManager class
 * @version 0.1
 * @date 2023-12-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef SDLMANAGER_HPP
#define SDLMANAGER_HPP

#include <memory>
#include <mutex>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

class SDLManager {
private:
	SDLManager() :
		sdl(SDL_INIT_VIDEO),
		window("Caption", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 480, 360, 0), // FIXME
		renderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)
	{}
	~SDLManager() {}
public:
	static SDLManager& get() {
		static SDLManager instance; // Scott Meyer's Singleton
		return instance;
	}

	// Copy constructor - disable
	SDLManager(SDLManager& other) = delete;
	// Copy assignment operator - disable
	SDLManager& operator=(SDLManager& other) = delete;

private:
	SDL_Rect m_invalidRect;

	bool needsRepaint() const;
public:
	SDL2pp::SDL sdl;
	SDL2pp::Window window;
	SDL2pp::Renderer renderer;

	void runEventLoop(void (*eventCapturer)(SDL_Event& ev), void (*paintEventCapturer)(SDL_Rect& invalidRect));
	void invalidateRect(SDL_Rect& rect);
};

#endif // SDLMANAGER_HPP
