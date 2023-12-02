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

#include "types.hpp"

class ICanvas {
public:
	virtual ~ICanvas() {}
	virtual int getWidth() = 0;
	virtual int getHeight() = 0;

	virtual Color getFillingColor() = 0;
	virtual Color getStrokingColor() = 0;
	virtual void setFillingColor(Color color) = 0;
	virtual void setStrokingColor(Color color) = 0;

	virtual double getStrokeWidth() = 0;
	virtual void setStrokeWidth(double value) = 0;

	virtual void fillEllipse(int x, int y, int rx, int ry) = 0;
	virtual void strokeEllipse(int x, int y, int rx, int ry) = 0;
	virtual void drawEllipse(int x, int y, int rx, int ry) = 0;

	virtual void fillCircle(int x, int y, int r) = 0;
	virtual void strokeCircle(int x, int y, int r) = 0;
	virtual void drawCircle(int x, int y, int r) = 0;
};

#endif // ICANVAS_HPP
