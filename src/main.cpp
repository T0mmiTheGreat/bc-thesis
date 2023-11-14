/**
 * @file main.cpp
 * @author Tomáš Ludrovan
 * @version 0.1
 * @date 2023-11-14
 */
#include <memory>

#include <SDL2/SDL.h> // SDL_main

#include "scene.h"
#include "sprite.h"

extern "C"
int main(int argc, char *argv[])
{
	auto scene = Scene();
	auto sprite = std::make_shared<SpriteCircle>(scene);
	sprite->setX(120);
	sprite->setY(140);
	scene.add_sprite(sprite);
	scene.paint();
	bool running = true;
	while (running) {
		SDL_Event ev;
		SDL_WaitEvent(&ev);
		if (ev.type == SDL_QUIT) {
			running = false;
		}
		else if (ev.type == SDL_KEYDOWN) {
			switch (ev.key.keysym.sym) {
				case SDLK_LEFT: {
					sprite->setX(sprite->getX() - 3);
				} break;
				case SDLK_RIGHT: {
					sprite->setX(sprite->getX() + 3);
				} break;
				case SDLK_UP: {
					sprite->setY(sprite->getY() - 3);
				} break;
				case SDLK_DOWN: {
					sprite->setY(sprite->getY() + 3);
				} break;
			}
		}
		scene.paint();
	}
	return 0;
}
