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
#include "sysproxy/SysProxyFactory.hpp"

class MySubscriber : public ISysSubscriber {
private:
	int spriteX;
	int spriteY;
	int spriteR;
public:
	MySubscriber() : spriteX{120}, spriteY{140}, spriteR{36} {}

	void keyDownEvent(KeyCode key) override {
		switch (key) {
			case KEY_LEFT_ARROW:
				spriteX -= 3;
				break;
			case KEY_UP_ARROW:
				spriteY -= 3;
				break;
			case KEY_RIGHT_ARROW:
				spriteX += 3;
				break;
			case KEY_DOWN_ARROW:
				spriteY += 3;
				break;
		}
		SysProxyFactory::createDefault()->invalidateRect();
	}
	void mouseBtnDownEvent(MouseBtn btn) override {}
	void mouseMoveEvent(int x, int y) override {}
	void startEvent() override {
		SysProxyFactory::createDefault()->invalidateRect();
	}
	void quitEvent() override {}
	void frameEvent() override {}
	void paintEvent(std::shared_ptr<ICanvas> canvas, Rect& invalidRect) override {
		canvas->setFillingColor(Color(0xff, 0, 0, 0xff));
		canvas->drawCircle(spriteX, spriteY, spriteR);
	}
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
