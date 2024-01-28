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

void SDLCanvas::polygonToVertexArrays(const PolygonF& pog,
	std::vector<Sint16>& vx, std::vector<Sint16>& vy)
{
	vx.reserve(pog.cornerCount());
	vy.reserve(pog.cornerCount());

	for (auto& corner : pog.corners) {
		vx.push_back(static_cast<Sint16>(corner.x));
		vy.push_back(static_cast<Sint16>(corner.y));
	}
}

void SDLCanvas::fillPolygonInternal(const Sint16* vx, const Sint16* vy, int n)
{
	SDL2pp::Color cl = fillToColor();

	filledPolygonRGBA(
		SDLManager::get().renderer.Get(),
		vx, vy, n,
		cl.r, cl.g, cl.b, cl.a
	);
}

void SDLCanvas::strokePolygonInternal(const Sint16* vx, const Sint16* vy, int n)
{
	SDL2pp::Color cl = strokeToColor();

	aapolygonRGBA(
		SDLManager::get().renderer.Get(),
		vx, vy, n,
		cl.r, cl.g, cl.b, cl.a
	);
}

Size2d SDLCanvas::getImageSize(ImageId img)
{
	SDL2pp::Point p = SDLManager::get().getImageSize(img);
	return SDLManager::sdlPointToSize(p);
}

SDL2pp::Color SDLCanvas::fillToColor()
{
	SDL2pp::Color res = SDLManager::colorToSdlColor(getFillingColorMod());
	return res;
}

SDL2pp::Color SDLCanvas::strokeToColor()
{
	SDL2pp::Color res = SDLManager::colorToSdlColor(getStrokingColorMod());
	return res;
}

int SDLCanvas::getWidth()
{
	int res = SDLManager::get().window.GetDrawableWidth();
	return res;
}

int SDLCanvas::getHeight()
{
	int res = SDLManager::get().window.GetDrawableHeight();
	return res;
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

void SDLCanvas::strokeLine(int x0, int y0, int x1, int y1)
{
	SDLManager::get().renderer.SetDrawColor(strokeToColor());
	SDLManager::get().renderer.DrawLine(
		SDL2pp::Point(x0, y0),
		SDL2pp::Point(x1, y1)
	);
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

void SDLCanvas::fillPolygon(const PolygonF& pog)
{
	std::vector<Sint16> vx, vy;
	polygonToVertexArrays(pog, vx, vy);
	fillPolygonInternal(vx.data(), vy.data(), pog.cornerCount());
}

void SDLCanvas::strokePolygon(const PolygonF& pog)
{
	std::vector<Sint16> vx, vy;
	polygonToVertexArrays(pog, vx, vy);
	strokePolygonInternal(vx.data(), vy.data(), pog.cornerCount());
}

void SDLCanvas::drawPolygon(const PolygonF & pog)
{
	std::vector<Sint16> vx, vy;
	polygonToVertexArrays(pog, vx, vy);
	fillPolygonInternal(vx.data(), vy.data(), pog.cornerCount());
	strokePolygonInternal(vx.data(), vy.data(), pog.cornerCount());
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

void SDLCanvas::copyImage(ImageId img, const Rect& srcRect, const Rect& dstRect)
{
	// Get the texture object
	SDL2pp::Texture& img_ = SDLManager::get().getImage(img);

	// Set texture color modulation
	SDL2pp::Color colorMod_(colorMod.r, colorMod.g, colorMod.b, colorMod.a);
	img_.SetColorAndAlphaMod(colorMod_);

	// Convert rectangles
	SDL2pp::Rect srcRect_ = SDLManager::rectToSdlRect(srcRect);
	SDL2pp::Rect dstRect_ = SDLManager::rectToSdlRect(dstRect);
	
	// Render
	SDLManager::get().renderer.Copy(img_, srcRect_, dstRect_);

	// Reset texture color modulation
	img_.SetColorAndAlphaMod();
}
