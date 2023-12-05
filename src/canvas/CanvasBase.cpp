/**
 * @file CanvasBase.cpp
 * @author Tomáš Ludrovan
 * @brief CanvasBase class
 * @version 0.1
 * @date 2023-12-02
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "canvas/CanvasBase.hpp"

int CanvasBase::getWidth()
{
	return getRect().w;
}

int CanvasBase::getHeight()
{
	return getRect().h;
}

Color CanvasBase::getFillingColor()
{
	return fColor;
}

Color CanvasBase::getStrokingColor()
{
	return sColor;
}

void CanvasBase::setFillingColor(Color color)
{
	fColor = color;
}

void CanvasBase::setStrokingColor(Color color)
{
	sColor = color;
}

double CanvasBase::getStrokeWidth()
{
	return sWidth;
}

void CanvasBase::setStrokeWidth(double value)
{
	sWidth = value;
}

void CanvasBase::drawEllipse(int x, int y, int rx, int ry)
{
	fillEllipse(x, y, rx, ry);
	strokeEllipse(x, y, rx, ry);
}

void CanvasBase::drawCircle(int x, int y, int r)
{
	fillCircle(x, y, r);
	strokeCircle(x, y, r);
}

void CanvasBase::drawRectangle(int x, int y, int w, int h)
{
	fillRectangle(x, y, w, h);
	strokeRectangle(x, y, w, h);
}
