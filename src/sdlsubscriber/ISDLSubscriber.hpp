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

class ISDLSubscriber {
public:
	virtual ~ISDLSubscriber() {}
	virtual void generalEvent(SDL_Event& ev) = 0;
	virtual void paintEvent(SDL_Rect& invalidRect) = 0;
	virtual void frameEvent() = 0;
};

#endif // ISDLSUBSCRIBER_HPP
