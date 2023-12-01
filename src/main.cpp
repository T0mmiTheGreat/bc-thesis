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

#include "scene.h"
#include "sprite.h"
#include "syssubscriber/ISysSubscriber.hpp"
#include "sdlmanager/SDLManager.hpp"
#include "sdlsubscriber/SDLSubscriber.hpp"

class MySubscriber : public ISysSubscriber {
private:
	Scene scene;
	std::shared_ptr<SpriteCircle> sprite;
public:
	MySubscriber() : scene(), sprite{std::make_shared<SpriteCircle>(scene)} {
		sprite->setX(120);
		sprite->setY(140);
		scene.add_sprite(sprite);
		scene.paint();
	}

	void keyDownEvent(KeyCode key) override {
		switch (key) {
			case KEY_LEFT_ARROW:
				sprite->setX(sprite->getX() - 3);
				break;
			case KEY_UP_ARROW:
				sprite->setY(sprite->getY() - 3);
				break;
			case KEY_RIGHT_ARROW:
				sprite->setX(sprite->getX() + 3);
				break;
			case KEY_DOWN_ARROW:
				sprite->setY(sprite->getY() + 3);
				break;
		}
		scene.paint();
	}
	void mouseBtnDownEvent(MouseBtn btn) override {}
	void mouseMoveEvent(int x, int y) override {}
	void startEvent() override {}
	void quitEvent() override {}
	void frameEvent() override {}
};

extern "C"
int main(int argc, char *argv[])
{
	auto subs = std::make_shared<MySubscriber>();
	auto pubs = std::make_shared<SDLSubscriber>(subs);
	SDLManager::get().assignSubscriber(pubs);
	SDLManager::get().runEventLoop();
	return 0;
}
