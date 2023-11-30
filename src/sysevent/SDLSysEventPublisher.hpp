/**
 * @file SDLSysEventPublisher.hpp
 * @author Tomáš Ludrovan
 * @brief SDLSysEventPublisher class
 * @version 0.1
 * @date 2023-11-30
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef SDLSYSEVENTPUBLISHER_HPP
#define SDLSYSEVENTPUBLISHER_HPP

#include <SDL2/SDL.h>

#include "sysevent/SysEventTypes.hpp"
#include "sysevent/SysEventPublisherBase.hpp"

class SDLSysEventPublisher : public SysEventPublisherBase {
protected:
	/**
	 * @brief Converts SDL key code to KeyCode enum.
	 */
	KeyCode sdlKeycodeToEnum(SDL_Keycode sdlk);
	/**
	 * @brief Converts SDL mouse button code to MouseBtn enum.
	 */
	MouseBtn sdlMouseBtnToEnum(Uint8 btn);
public:
	virtual void run(std::weak_ptr<ISysEventSubscriber> subscriber) override;
};

#endif // SDLSYSEVENTPUBLISHER_HPP
