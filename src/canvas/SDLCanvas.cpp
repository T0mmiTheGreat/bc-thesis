/**
 * @file SDLCanvas.cpp
 * @author Tomáš Ludrovan
 * @brief SDLCanvas class
 * @version 0.1
 * @date 2023-12-02
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "canvas/SDLCanvas.hpp"

#include <SDL2/SDL.h>
#include <SDL2pp/SDL.hh>
#include "SDL2_gfxPrimitives.h"

#include "sdlmanager/SDLManager.hpp"

SDL2pp::Color SDLCanvas::fillToColor()
{
	return SDL2pp::Color(fColor.r, fColor.g, fColor.b, fColor.a);
}

SDL2pp::Color SDLCanvas::strokeToColor()
{
	return SDL2pp::Color(sColor.r, sColor.g, sColor.b, sColor.a);
}

int SDLCanvas::getWidth()
{
	return SDLManager::get().window.GetDrawableWidth();
}

int SDLCanvas::getHeight()
{
	return SDLManager::get().window.GetDrawableHeight();
}

Rect SDLCanvas::getRect()
{
	SDL2pp::Point canvSize = SDLManager::get().window.GetDrawableSize();
	return Rect(0, 0, canvSize.GetX(), canvSize.GetY());
}

void SDLCanvas::setFillingColor(Color color)
{
	CanvasBase::setFillingColor(color);
}

void SDLCanvas::setStrokingColor(Color color)
{
	CanvasBase::setStrokingColor(color);
}

void SDLCanvas::fillEllipse(int x, int y, int rx, int ry)
{
	filledEllipseRGBA(
		SDLManager::get().renderer.Get(),
		x, y, rx, ry,
		fColor.r, fColor.g, fColor.b, fColor.a
	);
}

void SDLCanvas::strokeEllipse(int x, int y, int rx, int ry)
{
	aaellipseRGBA(
		SDLManager::get().renderer.Get(),
		x, y, rx, ry,
		sColor.r, sColor.g, sColor.b, sColor.a
	);
}

void SDLCanvas::drawEllipse(int x, int y, int rx, int ry)
{
	CanvasBase::drawEllipse(x, y, rx, ry);
}

void SDLCanvas::fillCircle(int x, int y, int r)
{
	filledCircleRGBA(
		SDLManager::get().renderer.Get(),
		x, y, r,
		fColor.r, fColor.g, fColor.b, fColor.a
	);
}

void SDLCanvas::strokeCircle(int x, int y, int r)
{
	aacircleRGBA(
		SDLManager::get().renderer.Get(),
		x, y, r,
		sColor.r, sColor.g, sColor.b, sColor.a
	);
}

void SDLCanvas::drawCircle(int x, int y, int r)
{
	CanvasBase::drawCircle(x, y, r);
}

void SDLCanvas::fillRectangle(int x, int y, int w, int h)
{
	SDLManager::get().renderer.SetDrawColor(fillToColor());
	SDLManager::get().renderer.FillRect(SDL2pp::Rect(x, y, w, h));
}

void SDLCanvas::strokeRectangle(int x, int y, int w, int h)
{
	SDLManager::get().renderer.SetDrawColor(strokeToColor());
	SDLManager::get().renderer.DrawRect(SDL2pp::Rect(x, y, w, h));
}

void SDLCanvas::fillText(int x, int y, const std::string& text, FontId font)
{
	if (text.empty()) {
		return;
	}

	// Get the font object
	SDL2pp::Font& fontObj = SDLManager::get().getFont(font);
	// Paint the text onto the surface
	auto textSurf = fontObj.RenderUTF8_Solid(text, fillToColor());
	// Convert the surface to texture
	SDL2pp::Texture textTex(SDLManager::get().renderer, textSurf);
	// Render the texture
	SDLManager::get().renderer.Copy(textTex, SDL2pp::NullOpt, SDL2pp::Point(x, y));
}

