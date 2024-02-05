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

	updateBlendModeByFill();
	filledPolygonRGBA(
		SDLManager::get().renderer.Get(),
		vx, vy, n,
		cl.r, cl.g, cl.b, cl.a
	);
}

void SDLCanvas::strokePolygonInternal(const Sint16* vx, const Sint16* vy, int n)
{
	SDL2pp::Color cl = strokeToColor();

	updateBlendModeByStroke();
	aapolygonRGBA(
		SDLManager::get().renderer.Get(),
		vx, vy, n,
		cl.r, cl.g, cl.b, cl.a
	);
}

void SDLCanvas::updateBlendModeByStroke()
{
	setBlendModeByColor(sColor);
}

void SDLCanvas::updateBlendModeByFill()
{
	setBlendModeByColor(fColor);
}

void SDLCanvas::setBlendModeByColor(const Color& color)
{
	SDL_BlendMode mode = (color.a == 0xff ? SDL_BLENDMODE_NONE : SDL_BLENDMODE_BLEND);
	SDLManager::get().renderer.SetDrawBlendMode(mode);
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
	updateBlendModeByStroke();
	SDLManager::get().renderer.DrawLine(
		SDL2pp::Point(x0, y0),
		SDL2pp::Point(x1, y1)
	);
}

void SDLCanvas::dashedLine(int x0, int y0, int x1, int y1)
{
	// The result is a dashed line with the dash length and the space length
	// both equal to 10. At (x1, y1) is either trimmed dash or a space. At
	// (x0, y0) is usually full-length dash, unless the distance between
	// (x0, y0) and (x1, y1) is less than 10.

	// The algorithm:
	//   1) Calculate the `dashSpaceCountF` value -- the number of dashes and
	//      spaces -- by dividing the distance between the end points and
	//      dividing the distance by the dash length.
	//   2) Calculate the vector `(dx, dy)` -- vector between dash or space
	//      endpoints -- by dividing the vector `(x1-x0, y1-y0)` by `nds` value.
	//   3) Draw the dashes by utilizing the `(dx, dy)` vector (it is rather
	//      obvious how, and I'm not going to expand on this here).

	// Also equal to space length
	static constexpr int DASH_LENGTH = 10;

	PointF p0F(x0, y0);
	PointF p1F(x1, y1);

	// Distance between (x0, y0) -> (x1, y1)
	double lineLength = sqrt(p0F.sqrDistance(p1F));
	// Number of dashes + number of spaces. Also includes fraction of the last,
	// trimmed one.
	double dashSpaceCountF = lineLength / DASH_LENGTH;
	// Number of dashes + number of spaces. Also includes the last, trimmed one
	// (whole, not just fraction).
	int dashSpaceCount = static_cast<int>(std::ceil(dashSpaceCountF));

	// Vector (x0, y0) -> (x1, y1)
	PointF v = p1F - p0F;
	// Length of a dash or space along X axis
	double dx = v.x / dashSpaceCountF;
	// Length of a dash or space along Y axis
	double dy = v.y / dashSpaceCountF;

	SDLManager::get().renderer.SetDrawColor(strokeToColor());
	updateBlendModeByStroke();
	SDL2pp::Renderer& renderer = SDLManager::get().renderer;

	// Dash origin
	PointF dashP0F = p0F;
	// Dash end point
	PointF dashP1F;

	// Dash coordinates converted to integers
	SDL2pp::Point dashP0, dashP1;

	// In one iteration we draw the dash and skip the space
	for (int i = 0; i < (dashSpaceCount + 1) / 2; i++) {
		// Place the end point
		dashP1F = dashP0F;
		dashP1F.x += dx;
		dashP1F.y += dy;

		// Must not go past the end point of the line (x1, y1).
		// We know that the dash origin will never be past the line end point.
		// That means if the X of the origin and end point of the dash are both
		// less or both greater than the line end point, the dash end point did
		// not go past the end point of the line. Same applies for Y.
		if ((dashP0F.x < p1F.x) != (dashP1F.x < p1F.x)
		 || (dashP0F.y < p1F.y) != (dashP1F.y < p1F.y)) {
			dashP1F = p1F;
		}

		// Cast
		dashP0.x = static_cast<int>(dashP0F.x);
		dashP0.y = static_cast<int>(dashP0F.y);
		dashP1.x = static_cast<int>(dashP1F.x);
		dashP1.y = static_cast<int>(dashP1F.y);
		
		renderer.DrawLine(dashP0, dashP1);

		// Set the next dash origin -- skip the current dash, skip the space
		// after the dash
		dashP0F.x = dashP1F.x + dx;
		dashP0F.y = dashP1F.y + dy;
	}
}

void SDLCanvas::fillEllipse(int x, int y, int rx, int ry)
{
	updateBlendModeByFill();
	filledEllipseRGBA(
		SDLManager::get().renderer.Get(),
		x, y, rx, ry,
		fColor.r, fColor.g, fColor.b, fColor.a
	);
}

void SDLCanvas::strokeEllipse(int x, int y, int rx, int ry)
{
	updateBlendModeByStroke();
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
	updateBlendModeByFill();
	filledCircleRGBA(
		SDLManager::get().renderer.Get(),
		x, y, r,
		fColor.r, fColor.g, fColor.b, fColor.a
	);
}

void SDLCanvas::strokeCircle(int x, int y, int r)
{
	updateBlendModeByStroke();
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

void SDLCanvas::dashedCircle(int x, int y, int r)
{
	static constexpr int DASH_COUNT = 20;
	static constexpr int PATTERN_LENGTH = 360 / DASH_COUNT;
	static constexpr int DASH_LENGTH = PATTERN_LENGTH / 2;

	Color cl = sColor.getModulated(colorMod);
	
	updateBlendModeByStroke();

	for (int i = 0, dashStart = 0; i < DASH_COUNT; i++, dashStart += PATTERN_LENGTH) {
		arcRGBA(
			SDLManager::get().renderer.Get(),
			x, y, r,
			dashStart, dashStart + DASH_LENGTH,
			cl.r, cl.g, cl.b, cl.a
		);
	}
}

void SDLCanvas::fillRectangle(int x, int y, int w, int h)
{
	SDLManager::get().renderer.SetDrawColor(fillToColor());
	updateBlendModeByFill();
	SDLManager::get().renderer.FillRect(SDL2pp::Rect(x, y, w, h));
}

void SDLCanvas::strokeRectangle(int x, int y, int w, int h)
{
	SDLManager::get().renderer.SetDrawColor(strokeToColor());
	updateBlendModeByStroke();
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

	updateBlendModeByFill();

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
