/**
 * @file sprite.cpp
 * @author Tomáš Ludrovan
 * @brief Sprite class implementation
 * @version 0.1
 * @date 2023-11-14
 */
#include "sprite.h"

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>
#include "SDL2_gfxPrimitives.h"

#include "scene.h"
#include "SDLManager.hpp"

void Sprite::setX(int value)
{
	// Invalidate the area where the sprite was
	SDL_Rect bounds;
	getBounds(&bounds);
	parent.invalidate(&bounds);

	// Move
	x = value;
	
	// Invalidate the area where the sprite is now
	bounds.x = value;
	parent.invalidate(&bounds);
}

void Sprite::setY(int value)
{
	// Invalidate the area where the sprite was
	SDL_Rect bounds;
	getBounds(&bounds);
	parent.invalidate(&bounds);

	// Move
	y = value;
	
	// Invalidate the area where the sprite is now
	bounds.y = value;
	parent.invalidate(&bounds);
}

void SpriteEllipsis::render()
{
	filledEllipseRGBA(SDLManager::get().renderer.Get(), x, y, W, H, 0xff, 0, 0, 0xff);
	aaellipseRGBA(SDLManager::get().renderer.Get(), x, y, W, H, 0xff, 0, 0, 0xff);
}

void SpriteEllipsis::getBounds(SDL_Rect *out_bounds)
{
	*out_bounds = SDL_Rect{
		.x = x,
		.y = y,
		.w = W,
		.h = H
	};
}

void SpriteCircle::render()
{
	filledCircleRGBA(SDLManager::get().renderer.Get(), x, y, R, 0xff, 0, 0, 0xff);
	aacircleRGBA(SDLManager::get().renderer.Get(), x, y, R, 0xff, 0, 0, 0xff);
}

void SpriteCircle::getBounds(SDL_Rect *out_bounds)
{
	*out_bounds = SDL_Rect{
		.x = x,
		.y = y,
		.w = R,
		.h = R
	};
}
