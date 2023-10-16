#include <iostream>

#include <SDL2/SDL.h> // SDL_main

#include "scene.h"

extern "C"
int main(int argc, char *argv[])
{
	auto scene = Scene();
	scene.paint();
	bool running = true;
	while (running) {
		SDL_Event ev;
		SDL_WaitEvent(&ev);
		if (ev.type == SDL_QUIT) {
			running = false;
		}
	}
	return 0;
}
