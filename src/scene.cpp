#include "scene.h"

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>


SDL2pp::SDL Scene::sdl(SDL_INIT_VIDEO);
SDL2pp::Window Scene::win("Caption", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 480, 360, 0);
SDL2pp::Renderer Scene::renderer(Scene::win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

void Scene::paint()
{
	renderer.SetDrawColor();
	renderer.Clear();
	renderer.Present();
}
