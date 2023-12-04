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
#include "SDL2_gfxPrimitives.h"

#include "sdlmanager/SDLManager.hpp"

int SDLCanvas::getWidth()
{
	return SDLManager::get().window.GetDrawableWidth();
}

int SDLCanvas::getHeight()
{
	return SDLManager::get().window.GetDrawableHeight();
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

