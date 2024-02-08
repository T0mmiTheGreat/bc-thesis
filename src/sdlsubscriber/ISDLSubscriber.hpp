/**
 * @file ISDLSubscriber.hpp
 * @author Tomáš Ludrovan
 * @brief ISDLSubscriber interface
 * @version 0.1
 * @date 2023-12-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef ISDLSUBSCRIBER_HPP
#define ISDLSUBSCRIBER_HPP

#include <SDL2/SDL.h>

/**
 * @brief Defines events created by event loop.
 */
class ISDLSubscriber {
public:
	virtual ~ISDLSubscriber() {}
	/**
	 * @brief Event acquired by polling events.
	 */
	virtual void generalEvent(const SDL_Event& ev) = 0;
	/**
	 * @brief Request to paint.
	 * 
	 * @details This event happens after SDLManager received invalidateRect()
	 *          request.
	 * 
	 * @param invalidRect Area to repaint.
	 */
	virtual void paintEvent(const SDL_Rect& invalidRect) = 0;
	/**
	 * @brief Event that happens every event loop iteration.
	 */
	virtual void loopEvent() = 0;
	/**
	 * @brief The event loop has started.
	 * 
	 * @details This is the first event of the event loop and happens only once.
	 */
	virtual void startEvent() = 0;
};

#endif // ISDLSUBSCRIBER_HPP
