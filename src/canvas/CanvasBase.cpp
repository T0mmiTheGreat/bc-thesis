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

Size2d CanvasBase::getImageSize(ImageId img)
{
	return Size2d();
}

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

void CanvasBase::copyImage(ImageId img, int x, int y)
{
	Size2d s = getImageSize(img);
	copyImage(img, Rect(0, 0, s), x, y);
}

void CanvasBase::copyImage(ImageId img, int x, int y, double scale)
{
	Size2d s = getImageSize(img);
	copyImage(img, Rect(0, 0, s), x, y, scale);
}

void CanvasBase::copyImage(ImageId img, int x, int y, double xScale, double yScale)
{
	Size2d s = getImageSize(img);
	copyImage(img, Rect(0, 0, s), x, y, xScale, yScale);
}

void CanvasBase::copyImage(ImageId img, const Rect& srcRect, int x, int y)
{
	copyImage(img, srcRect, x, y, 1.0);
}

void CanvasBase::copyImage(ImageId img, const Rect& srcRect, int x, int y,
	double scale)
{
	copyImage(img, srcRect, x, y, scale, scale);
}

void CanvasBase::copyImage(ImageId img, const Rect& srcRect, int x, int y,
	double xScale, double yScale)
{
	const Rect dstRect(x, y, static_cast<int>(srcRect.w * xScale),
		static_cast<int>(srcRect.h * yScale));
	copyImage(img, srcRect, dstRect);
}
