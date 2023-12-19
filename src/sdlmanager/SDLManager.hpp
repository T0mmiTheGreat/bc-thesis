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

#include <array>
#include <memory>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>
#include <SDL2pp/SDLTTF.hh>
#include <SDL2pp/Font.hh>

#include "sdlsubscriber/ISDLSubscriber.hpp"
#include "types.hpp"

/**
 * @brief Singleton class for the SDL-related operations.
 */
class SDLManager {
private:
	// It's singleton; constructor and destructor are private

	SDLManager();
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

	/**
	 * @brief Global SDL object instance.
	 */
	SDL2pp::SDL sdl;
	SDL2pp::SDLTTF sdlTtf;
	/**
	 * @brief Global window instance.
	 */
	SDL2pp::Window window;
	/**
	 * @brief Global renderer instance.
	 */
	SDL2pp::Renderer renderer;
private:
	/**
	 * @brief The assigned SDL event subscriber.
	 */
	std::shared_ptr<ISDLSubscriber> m_subscriber;
	/**
	 * @brief State of the event loop.
	 */
	EventLoopState m_eventLoopstate;
	/**
	 * @brief The area which needs to be repainted.
	 */
	SDL_Rect m_invalidRect;
	std::array<SDL2pp::Font, fontIdCount> m_fonts;

	/**
	 * @brief Returns true if the window needs to be repainted.
	 */
	bool needsRepaint() const { return !SDL_RectEmpty(&m_invalidRect); }
public:

	/**
	 * @brief Getter for the `m_eventLoopstate` variable.
	 */
	EventLoopState getEventLoopState() const { return m_eventLoopstate; }
	SDL2pp::Font& getFont(FontId id) { return m_fonts[id]; }

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
	static Size2d sdlPointToSize(const SDL2pp::Point& point);
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
