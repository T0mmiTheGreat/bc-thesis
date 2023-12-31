/**
 * @file CanvasBase.hpp
 * @author Tomáš Ludrovan
 * @brief CanvasBase class
 * @version 0.1
 * @date 2023-12-02
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef CANVASBASE_HPP
#define CANVASBASE_HPP

#include "canvas/ICanvas.hpp"
#include "types.hpp"

/**
 * @brief The base class for canvases.
 * 
 * @details Provides some convenient definitions for descendants.
 * 
 *          The getWidth()/getHeight() methods use the getRect() method to
 *          obtain the canvas dimensions.
 * 
 *          The filling color, stroking color and stroke width variables are
 *          defined, along with the definitions of getters and setters for
 *          them.
 * 
 *          If the descendants don't have a specific way of filling and stroking
 *          shapes at the same time, this class defines the draw<Sth>() methods
 *          to call fill<Sth>() and then stroke<Sth>() so the descendants don't
 *          have to.
 */
class CanvasBase : public ICanvas {
protected:
	/**
	 * @brief Color used for filling operations.
	 */
	Color fColor;
	/**
	 * @brief Color used for stroking operations.
	 */
	Color sColor;
	/**
	 * @brief Stroke width.
	 */
	double sWidth;
public:
	/**
	 * @brief Get width of the canvas in screen coordinates.
	 * 
	 * @details Uses `getRect()` to obtain the width.
	 */
	virtual int getWidth() override;
	/**
	 * @brief Get height of the canvas in screen coordinates.
	 * 
	 * @details Uses `getRect()` to obtain the height.
	 */
	virtual int getHeight() override;

	/**
	 * @brief Get the current color used for filling operations.
	 * 
	 * @details Returns the `fColor` value.
	 */
	virtual Color getFillingColor() override;
	/**
	 * @brief Get the current color used for stroking operations.
	 * 
	 * @details Returns the `sColor` value.
	 */
	virtual Color getStrokingColor() override;
	/**
	 * @brief Set the color for use in filling operations.
	 * 
	 * @details Modifies the `fColor` value.
	 */
	virtual void setFillingColor(Color color) override;
	/**
	 * @brief Set the color for use in stroking operations.
	 * 
	 * @details Modifies the `sColor` value.
	 */
	virtual void setStrokingColor(Color color) override;

	/**
	 * @brief Get the stroke thickness in stroking operations.
	 * 
	 * @details Returns the `sWidth` value.
	 */
	virtual double getStrokeWidth() override;
	/**
	 * @brief Set the stroke thickness for stroking operations.
	 * 
	 * @details Modifies the `sWidth` value.
	 */
	virtual void setStrokeWidth(double value) override;

	/**
	 * @brief Fill and stroke an elliptical area.
	 * 
	 * @details Calls `fillEllipse()`, then `strokeEllipse()`.
	 * 
	 * @param x X coordinate of the ellipse center.
	 * @param y Y coordinate of the ellipse center.
	 * @param rx Horizontal radius of the ellipse.
	 * @param ry Vertical radius of the ellipse.
	 */
	virtual void drawEllipse(int x, int y, int rx, int ry) override;
	/**
	 * @brief Fill and stroke a circular area.
	 * 
	 * @details Calls `fiillCircle()`, then `strokeCircle()`.
	 * 
	 * @param x X coordinate of the circle center.
	 * @param y Y coordinate of the circle center.
	 * @param r Radius of the circle.
	 */
	virtual void drawCircle(int x, int y, int r) override;
	/**
	 * @brief Fill and stroke a rectangular area.
	 * 
	 * @details Calls `fillRectangle()`, then `strokeRectangle()`.
	 * 
	 * @param x X coordinate of the rectangle's top left corner.
	 * @param y Y coordinate of the rectangle's top left corner.
	 * @param w Width of the rectangle.
	 * @param h Height of the rectangle.
	 */
	virtual void drawRectangle(int x, int y, int w, int h) override;
};

#endif // CANVASBASE_HPP
