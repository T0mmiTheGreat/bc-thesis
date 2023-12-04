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

/**
 * @brief State of the event loop execution.
 */
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

/**
 * @brief Singleton class for the SDL-related operations.
 */
class SDLManager {
private:
	// It's singleton; constructor and destructor are private

	SDLManager() :
		sdl(SDL_INIT_VIDEO),
		window("Caption", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 480, 360, 0), // FIXME
		renderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC),
		m_state{MSTATE_PRERUN}
	{}
	~SDLManager() {}
public:
	/**
	 * @brief Returns instance of the SDLManager singleton.
	 */
	static SDLManager& get() {
		static SDLManager instance; // Scott Meyer's Singleton
		return instance;
	}

	// Copy constructor - disable
	SDLManager(SDLManager& other) = delete;
	// Copy assignment operator - disable
	SDLManager& operator=(SDLManager& other) = delete;

private:
	/**
	 * @brief The assigned SDL event subscriber.
	 */
	std::shared_ptr<ISDLSubscriber> m_subscriber;
	/**
	 * @brief State of the event loop.
	 */
	SDLManagerState m_state;
	/**
	 * @brief The area which needs to be repainted.
	 */
	SDL_Rect m_invalidRect;

	/**
	 * @brief Returns true if the window needs to be repainted.
	 */
	bool needsRepaint() const { return !SDL_RectEmpty(&m_invalidRect); }
public:
	/**
	 * @brief Global SDL object instance.
	 */
	SDL2pp::SDL sdl;
	/**
	 * @brief Global window instance.
	 */
	SDL2pp::Window window;
	/**
	 * @brief Global renderer instance.
	 */
	SDL2pp::Renderer renderer;

	/**
	 * @brief Getter for the `state` (`m_state`) variable.
	 */
	SDLManagerState getState() const { return m_state; }

	/**
	 * @brief Assigns a SDL event subscriber.
	 * 
	 * @details This method MUST always be called before running the event loop.
	 */
	void assignSubscriber(std::shared_ptr<ISDLSubscriber> subscriber) { m_subscriber = subscriber; }
	/**
	 * @brief Runs the event loop.
	 * 
	 * @details This functions returns once it receives the SDL_QUIT event. It
	 *          does, however, send the quitEvent to the listener first.
	 * 
	 *          Before calling, the subscriber MUST be assigned using the
	 *          assignSubscriber() method. The reason why the subscriber is not
	 *          passed as parameter to this method is because this method is
	 *          generally called from the SysProxy which has no knowledge about
	 *          controllers, let alone what sends them events.
	 */
	void runEventLoop();
	/**
	 * @brief Marks area of the window to be repainted.
	 */
	void invalidateRect(SDL_Rect& rect);
	
	/**
	 * @brief Converts SDL key code to KeyCode enum.
	 */
	static KeyCode sdlKeycodeToEnum(SDL_Keycode sdlk);
	/**
	 * @brief Converts SDL mouse button code to MouseBtn enum.
	 */
	static MouseBtn sdlMouseBtnToEnum(Uint8 btn);
	/**
	 * @brief Converts SDL rectangle to Rect type.
	 */
	static Rect sdlRectToRect(const SDL_Rect& rect);
	/**
	 * @brief Converts KeyCode enum to SDL key scancode.
	 */
	static SDL_Scancode enumToSdlScancode(KeyCode keyCode);
	/**
	 * @brief Converts Rect type to SDL rectangle.
	 */
	static SDL_Rect rectToSdlRect(const Rect& rect);
};

#endif // SDLMANAGER_HPP
