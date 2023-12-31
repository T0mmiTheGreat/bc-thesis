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
	/**
	 * @brief Converts `fColor` value to `SDL2pp::Color`.
	 */
	SDL2pp::Color fillToColor();
	/**
	 * @brief Converts `sColor` value to `SDL2pp::Color`.
	 */
	SDL2pp::Color strokeToColor();
public:
	/**
	 * @brief Get width of the canvas in screen coordinates.
	 */
	int getWidth() override;
	/**
	 * @brief Get height of the canvas in screen coordinates.
	 */
	int getHeight() override;
	/**
	 * @brief Get the canvas bounds.
	 */
	Rect getRect() override;

	/**
	 * @brief Set the color for use in filling operations.
	 */
	void setFillingColor(Color color) override;
	/**
	 * @brief Set the color for use in stroking operations.
	 */
	void setStrokingColor(Color color) override;

	/**
	 * @brief Fill an elliptical area.
	 * 
	 * @param x X coordinate of the ellipse center.
	 * @param y Y coordinate of the ellipse center.
	 * @param rx Horizontal radius of the ellipse.
	 * @param ry Vertical radius of the ellipse.
	 */
	void fillEllipse(int x, int y, int rx, int ry) override;
	/**
	 * @brief Stroke an elliptical area.
	 * 
	 * @param x X coordinate of the ellipse center.
	 * @param y Y coordinate of the ellipse center.
	 * @param rx Horizontal radius of the ellipse.
	 * @param ry Vertical radius of the ellipse.
	 */
	void strokeEllipse(int x, int y, int rx, int ry) override;
	/**
	 * @brief Fill and stroke an elliptical area.
	 * 
	 * @param x X coordinate of the ellipse center.
	 * @param y Y coordinate of the ellipse center.
	 * @param rx Horizontal radius of the ellipse.
	 * @param ry Vertical radius of the ellipse.
	 */
	void drawEllipse(int x, int y, int rx, int ry) override;

	/**
	 * @brief Fill a circular area.
	 * 
	 * @param x X coordinate of the circle center.
	 * @param y Y coordinate of the circle center.
	 * @param r Radius of the circle.
	 */
	void fillCircle(int x, int y, int r) override;
	/**
	 * @brief Stroke a circular area.
	 * 
	 * @param x X coordinate of the circle center.
	 * @param y Y coordinate of the circle center.
	 * @param r Radius of the circle.
	 */
	void strokeCircle(int x, int y, int r) override;
	/**
	 * @brief Fill and stroke a circular area.
	 * 
	 * @param x X coordinate of the circle center.
	 * @param y Y coordinate of the circle center.
	 * @param r Radius of the circle.
	 */
	void drawCircle(int x, int y, int r) override;

	/**
	 * @brief Fill a rectangular area.
	 * 
	 * @param x X coordinate of the rectangle's top left corner.
	 * @param y Y coordinate of the rectangle's top left corner.
	 * @param w Width of the rectangle.
	 * @param h Height of the rectangle.
	 */
	void fillRectangle(int x, int y, int w, int h) override;
	/**
	 * @brief Stroke a rectangular area.
	 * 
	 * @param x X coordinate of the rectangle's top left corner.
	 * @param y Y coordinate of the rectangle's top left corner.
	 * @param w Width of the rectangle.
	 * @param h Height of the rectangle.
	 */
	void strokeRectangle(int x, int y, int w, int h) override;

	/**
	 * @brief Paint text.
	 * 
	 * @note The operation uses fill color.
	 * 
	 * @param x X coordinate of the text area rectangle.
	 * @param y Y coordinate of the text area rectangle.
	 * @param text The text.
	 * @param font The font to use.
	 */
	void fillText(int x, int y, const std::string& text, FontId font) override;
};

#endif // SDLCANVAS_HPP
