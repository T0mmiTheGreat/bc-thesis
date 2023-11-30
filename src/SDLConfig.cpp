/**
 * @file SDLConfig.cpp
 * @author Tomáš Ludrovan
 * @brief Configuration for Simple DirectMedia Layer library
 * @version 0.1
 * @date 2023-11-30
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "SDLConfig.hpp"

#include <SDL2/SDL.h>
#include <SDL2pp/SDL.hh>

SDL2pp::SDL g_sdl(SDL_INIT_VIDEO);
SDL2pp::Window g_window("Caption", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 480, 360, 0); // FIXME
SDL2pp::Renderer g_renderer(g_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
