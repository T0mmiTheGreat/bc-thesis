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

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "sdlsubscriber/ISDLSubscriber.hpp"
#include "types.hpp"

enum SDLManagerState {
	// Can run event loop
	// Cannot call invalidateRect() and similar methods
	MSTATE_PRERUN,
	// Event loop is running, but cannot run another event loop
	// Can call invalidateRect() and similar methods
	MSTATE_RUNNING,
	// Event loop is finished and cannot start a new one
	// Cannot call invalidateRect() and similar methods
	MSTATE_POSTRUN,
};

class SDLManager {
private:
	SDLManager() :
		sdl(SDL_INIT_VIDEO),
		window("Caption", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 480, 360, 0), // FIXME
		renderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC),
		m_state{MSTATE_PRERUN}
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
	std::shared_ptr<ISDLSubscriber> m_subscriber;
	SDLManagerState m_state;
	SDL_Rect m_invalidRect;

	bool needsRepaint() const { return !SDL_RectEmpty(&m_invalidRect); }
public:
	SDL2pp::SDL sdl;
	SDL2pp::Window window;
	SDL2pp::Renderer renderer;

	SDLManagerState getState() const { return m_state; }

	void assignSubscriber(std::shared_ptr<ISDLSubscriber> subscriber) { m_subscriber = subscriber; }
	void runEventLoop();
	void invalidateRect(SDL_Rect& rect);
	
	/**
	 * @brief Converts SDL key code to KeyCode enum.
	 */
	static KeyCode sdlKeycodeToEnum(SDL_Keycode sdlk);
	/**
	 * @brief Converts SDL mouse button code to MouseBtn enum.
	 */
	static MouseBtn sdlMouseBtnToEnum(Uint8 btn);
	static Rect sdlRectToRect(const SDL_Rect& rect);
	static SDL_Scancode enumToSdlScancode(KeyCode keyCode);
	static SDL_Rect rectToSdlRect(const Rect& rect);
};

#endif // SDLMANAGER_HPP
