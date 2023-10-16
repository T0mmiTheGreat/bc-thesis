#ifndef SCENE_H
#define SCENE_H

#include <SDL2pp/SDL2pp.hh>


class Scene {
public:
	static SDL2pp::SDL sdl;
	static SDL2pp::Window win;
	static SDL2pp::Renderer renderer;

	void paint();
};

#endif // SCENE_H
