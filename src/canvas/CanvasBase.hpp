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
 *          If the descendants don't have a specific way of filling and
 *          stroking shapes at the same time, this class defines the drawSth()
 *          methods to call fillSth() and then strokeSth() so the descendants
 *          don't have to.
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
	virtual int getWidth() override;
	virtual int getHeight() override;

	virtual Color getFillingColor() override;
	virtual Color getStrokingColor() override;
	virtual void setFillingColor(Color color) override;
	virtual void setStrokingColor(Color color) override;

	virtual double getStrokeWidth() override;
	virtual void setStrokeWidth(double value) override;

	virtual void drawEllipse(int x, int y, int rx, int ry) override;
	virtual void drawCircle(int x, int y, int r) override;
	virtual void drawRectangle(int x, int y, int w, int h) override;
};

#endif // CANVASBASE_HPP
