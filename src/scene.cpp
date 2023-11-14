/**
 * @file scene.cpp
 * @author Tomáš Ludrovan
 * @brief Scene class implementation
 * @version 0.1
 * @date 2023-11-14
 */
#include "scene.h"

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>


SDL2pp::SDL Scene::sdl(SDL_INIT_VIDEO);
SDL2pp::Window Scene::win("Caption", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 480, 360, 0);
SDL2pp::Renderer Scene::renderer(Scene::win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

void Scene::invalidate(SDL_Rect *invalid_rect)
{
	(void)invalid_rect;
	is_invalid = true;
}

void Scene::paint()
{
	if (is_invalid) {
		renderer.SetDrawColor();
		renderer.Clear();
		for (auto sprite : sprites) {
			sprite->render();
		}
		renderer.Present();
	}
}

void Scene::add_sprite(std::shared_ptr<SceneSprite> sprite)
{
	sprites.push_back(sprite);
	SDL_Rect invalid_rect;
	sprite->getBounds(&invalid_rect);
	invalidate(&invalid_rect);
}
