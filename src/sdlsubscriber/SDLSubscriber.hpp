/**
 * @file SDLSubscriber.hpp
 * @author Tomáš Ludrovan
 * @brief SDLSubscriber class
 * @version 0.1
 * @date 2023-12-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef SDLSUBSCRIBER_HPP
#define SDLSUBSCRIBER_HPP

#include <memory>

#include "sdlsubscriber/ISDLSubscriber.hpp"
#include "syssubscriber/ISysSubscriber.hpp"

class SDLSubscriber : public ISDLSubscriber {
private:
	std::weak_ptr<ISysSubscriber> m_subscriber;
	
	/**
	 * @brief Converts SDL key code to KeyCode enum.
	 */
	KeyCode sdlKeycodeToEnum(SDL_Keycode sdlk);
	/**
	 * @brief Converts SDL mouse button code to MouseBtn enum.
	 */
	MouseBtn sdlMouseBtnToEnum(Uint8 btn);
public:
	SDLSubscriber(std::weak_ptr<ISysSubscriber> subscriber) :
		m_subscriber{subscriber}
	{}
	void generalEvent(SDL_Event& ev);
	void paintEvent(SDL_Rect& invalidRect);
	void frameEvent();
};

#endif // SDLSUBSCRIBER_HPP
