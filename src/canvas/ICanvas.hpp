/**
 * @file ICanvas.hpp
 * @author Tomáš Ludrovan
 * @brief ICanvas interface
 * @version 0.1
 * @date 2023-12-02
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef ICANVAS_HPP
#define ICANVAS_HPP

#include <string>

#include "types.hpp"

/**
 * @brief Library-independent canvas.
 * 
 * @remark The Y coordinate in drawing operations has the value of 0 at the
 *         canvas top border and grows towards the bottom border.
 */
class ICanvas {
public:
	virtual ~ICanvas() {}
	/**
	 * @brief Get width of the canvas in screen coordinates.
	 */
	virtual int getWidth() = 0;
	/**
	 * @brief Get height of the canvas in screen coordinates.
	 */
	virtual int getHeight() = 0;
	/**
	 * @brief Get the canvas bounds.
	 */
	virtual Rect getRect() = 0;

	/**
	 * @brief Get the current color used for filling operations.
	 */
	virtual Color getFillingColor() = 0;
	/**
	 * @brief Get the current color used for stroking operations.
	 */
	virtual Color getStrokingColor() = 0;
	/**
	 * @brief Set the color for use in filling operations.
	 */
	virtual void setFillingColor(Color color) = 0;
	/**
	 * @brief Set the color for use in stroking operations.
	 */
	virtual void setStrokingColor(Color color) = 0;

	/**
	 * @brief Get the stroke thickness in stroking operations.
	 */
	virtual double getStrokeWidth() = 0;
	/**
	 * @brief Set the stroke thickness for stroking operations.
	 */
	virtual void setStrokeWidth(double value) = 0;

	/**
	 * @brief Fill an elliptical area.
	 * 
	 * @param x X coordinate of the ellipse center.
	 * @param y Y coordinate of the ellipse center.
	 * @param rx Horizontal radius of the ellipse.
	 * @param ry Vertical radius of the ellipse.
	 */
	virtual void fillEllipse(int x, int y, int rx, int ry) = 0;
	/**
	 * @brief Stroke an elliptical area.
	 * 
	 * @param x X coordinate of the ellipse center.
	 * @param y Y coordinate of the ellipse center.
	 * @param rx Horizontal radius of the ellipse.
	 * @param ry Vertical radius of the ellipse.
	 */
	virtual void strokeEllipse(int x, int y, int rx, int ry) = 0;
	/**
	 * @brief Fill and stroke an elliptical area.
	 * 
	 * @param x X coordinate of the ellipse center.
	 * @param y Y coordinate of the ellipse center.
	 * @param rx Horizontal radius of the ellipse.
	 * @param ry Vertical radius of the ellipse.
	 */
	virtual void drawEllipse(int x, int y, int rx, int ry) = 0;

	/**
	 * @brief Fill a circular area.
	 * 
	 * @param x X coordinate of the circle center.
	 * @param y Y coordinate of the circle center.
	 * @param r Radius of the circle.
	 */
	virtual void fillCircle(int x, int y, int r) = 0;
	/**
	 * @brief Stroke a circular area.
	 * 
	 * @param x X coordinate of the circle center.
	 * @param y Y coordinate of the circle center.
	 * @param r Radius of the circle.
	 */
	virtual void strokeCircle(int x, int y, int r) = 0;
	/**
	 * @brief Fill and stroke a circular area.
	 * 
	 * @param x X coordinate of the circle center.
	 * @param y Y coordinate of the circle center.
	 * @param r Radius of the circle.
	 */
	virtual void drawCircle(int x, int y, int r) = 0;
	
	/**
	 * @brief Fill a rectangular area.
	 * 
	 * @param x X coordinate of the rectangle's top left corner.
	 * @param y Y coordinate of the rectangle's top left corner.
	 * @param w Width of the rectangle.
	 * @param h Height of the rectangle.
	 */
	virtual void fillRectangle(int x, int y, int w, int h) = 0;
	/**
	 * @brief Stroke a rectangular area.
	 * 
	 * @param x X coordinate of the rectangle's top left corner.
	 * @param y Y coordinate of the rectangle's top left corner.
	 * @param w Width of the rectangle.
	 * @param h Height of the rectangle.
	 */
	virtual void strokeRectangle(int x, int y, int w, int h) = 0;
	/**
	 * @brief Fill and stroke a rectangular area.
	 * 
	 * @param x X coordinate of the rectangle's top left corner.
	 * @param y Y coordinate of the rectangle's top left corner.
	 * @param w Width of the rectangle.
	 * @param h Height of the rectangle.
	 */
	virtual void drawRectangle(int x, int y, int w, int h) = 0;
	
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
	virtual void fillText(int x, int y, const std::string& text, FontId font) = 0;
};

#endif // ICANVAS_HPP
