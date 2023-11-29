#ifndef SDLSYSEVENTPUBLISHER_HPP
#define SDLSYSEVENTPUBLISHER_HPP

#include <SDL2/SDL.h>

#include "SysEventTypes.hpp"
#include "SysEventPublisherBase.hpp"

class SDLSysEventPublisher : public SysEventPublisherBase {
protected:
	KeyCode sdlKeycodeToEnum(SDL_Keycode sdlk);
	MouseBtn sdlMouseBtnToEnum(Uint8 btn);
public:
	virtual void run(std::weak_ptr<ISysEventSubscriber> subscriber) override;
};

#endif // SDLSYSEVENTPUBLISHER_HPP
