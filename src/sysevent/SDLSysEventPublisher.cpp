#include "SDLSysEventPublisher.hpp"

#include <SDL2/SDL.h>

KeyCode SDLSysEventPublisher::sdlKeycodeToEnum(SDL_Keycode sdlk)
{
	switch (sdlk) {
		case SDLK_RETURN: return KEY_RETURN;
		case SDLK_LEFT: return KEY_LEFT_ARROW;
		case SDLK_UP: return KEY_UP_ARROW;
		case SDLK_RIGHT: return KEY_RIGHT_ARROW;
		case SDLK_DOWN: return KEY_DOWN_ARROW;
		// TODO: more keys
		default: return KEY_UNKNOWN;
	}
}

MouseBtn SDLSysEventPublisher::sdlMouseBtnToEnum(Uint8 btn)
{
	switch (btn) {
		case SDL_BUTTON_LEFT: return BTN_LEFT;
		case SDL_BUTTON_MIDDLE: return BTN_MIDDLE;
		case SDL_BUTTON_RIGHT: return BTN_RIGHT;
		default: return BTN_UNKNOWN;
	}
}

void SDLSysEventPublisher::run(std::weak_ptr<ISysEventSubscriber> subscriber)
{
	SysEventPublisherBase::run(subscriber);
	SDL_Event ev;
	bool isRunning = true;
	while (isRunning) {
		while (SDL_PollEvent(&ev)) {
			switch (ev.type) {
				case SDL_KEYDOWN:
					keyDownEvent(sdlKeycodeToEnum(ev.key.keysym.sym));
					break;
				case SDL_MOUSEBUTTONDOWN:
					mouseBtnDownEvent(sdlMouseBtnToEnum(ev.button.button));
					break;
				case SDL_MOUSEMOTION:
					mouseMoveEvent(ev.motion.x, ev.motion.y);
					break;
				case SDL_QUIT:
					quitEvent();
					isRunning = false;
					break;
			}
		}
	}
}
