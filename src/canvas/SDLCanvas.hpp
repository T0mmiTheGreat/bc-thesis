/**
 * @file SDLCanvas.hpp
 * @author Tomáš Ludrovan
 * @brief SDLCanvas class
 * @version 0.1
 * @date 2023-12-02
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef SDLCANVAS_HPP
#define SDLCANVAS_HPP

#include <SDL2pp/SDL.hh>
#include <SDL2pp/Color.hh>

#include "canvas/CanvasBase.hpp"

/**
 * @brief Concrete implementation of ICanvas for the SDL library.
 */
class SDLCanvas : public CanvasBase {
public:
	SDL2pp::Color fillToColor();
	SDL2pp::Color strokeToColor();
public:
	int getWidth() override;
	int getHeight() override;
	Rect getRect() override;

	void setFillingColor(Color color) override;
	void setStrokingColor(Color color) override;

	void fillEllipse(int x, int y, int rx, int ry) override;
	void strokeEllipse(int x, int y, int rx, int ry) override;
	void drawEllipse(int x, int y, int rx, int ry) override;

	void fillCircle(int x, int y, int r) override;
	void strokeCircle(int x, int y, int r) override;
	void drawCircle(int x, int y, int r) override;

	void fillRectangle(int x, int y, int w, int h) override;
	void strokeRectangle(int x, int y, int w, int h) override;

	void fillText(int x, int y, const std::string& text, FontId font) override;
};

#endif // SDLCANVAS_HPP
