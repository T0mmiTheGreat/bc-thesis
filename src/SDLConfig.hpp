/**
 * @file SDLConfig.hpp
 * @author Tomáš Ludrovan
 * @brief Configuration for Simple DirectMedia Layer library
 * @version 0.1
 * @date 2023-11-30
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef SDLCONFIG_HPP
#define SDLCONFIG_HPP

#include <SDL2pp/SDL2pp.hh>

/**
 * @brief Global SDL library initialization object.
 */
extern SDL2pp::SDL g_sdl;
/**
 * @brief Global SDL window object.
 */
extern SDL2pp::Window g_window;
/**
 * @brief Global SDL renderer object.
 */
extern SDL2pp::Renderer g_renderer;

#endif // SDLCONFIG_HPP
