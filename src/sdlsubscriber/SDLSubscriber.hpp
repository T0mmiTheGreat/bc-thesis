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

/**
 * @brief Implementation of ISDLSubscriber.
 */
class SDLSubscriber : public ISDLSubscriber {
private:
	std::weak_ptr<ISysSubscriber> m_subscriber;
public:
	SDLSubscriber(std::weak_ptr<ISysSubscriber> subscriber)
		: m_subscriber{subscriber}
	{}
	virtual void generalEvent(const SDL_Event& ev) override;
	virtual void paintEvent(const SDL_Rect& invalidRect) override;
	virtual void loopEvent() override;
	virtual void startEvent() override;
};

#endif // SDLSUBSCRIBER_HPP
