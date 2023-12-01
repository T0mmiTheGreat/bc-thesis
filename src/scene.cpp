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

#include "sdlmanager/SDLManager.hpp"

void Scene::invalidate(SDL_Rect *invalid_rect)
{
	(void)invalid_rect;
	is_invalid = true;
}

void Scene::paint()
{
	if (is_invalid) {
		// Fill with black
		SDLManager::get().renderer.SetDrawColor();
		SDLManager::get().renderer.Clear();

		// Render sprites
		for (auto sprite : sprites) {
			sprite->render();
		}
		
		SDLManager::get().renderer.Present();
	}
}

void Scene::add_sprite(std::shared_ptr<SceneSprite> sprite)
{
	sprites.push_back(sprite);

	// Paint the sprite onto the scene immediatelly
	SDL_Rect invalid_rect;
	sprite->getBounds(&invalid_rect);
	invalidate(&invalid_rect);
}
