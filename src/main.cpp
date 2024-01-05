/**
 * @file main.cpp
 * @author Tomáš Ludrovan
 * @version 0.1
 * @date 2023-11-14
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <memory>

#include <SDL2/SDL.h> // SDL_main

#include "controller/ControllerFactory.hpp"
#include "sdlsubscriber/SDLSubscriber.hpp"
#include "sdlmanager/SDLManager.hpp"
#include "sysproxy/SysProxyFactory.hpp"

extern "C"
int main(int argc, char *argv[])
{
	auto rootCntl = ControllerFactory::createRootController(SysProxyFactory::createDefault());
	auto sdlSubs = std::make_shared<SDLSubscriber>(rootCntl);
	SDLManager::get().assignSubscriber(sdlSubs);
	rootCntl->run();
	return 0;
}
