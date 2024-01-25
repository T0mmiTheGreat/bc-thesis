/**
 * @file HorizontalLineSprite.cpp
 * @author Tomáš Ludrovan
 * @brief HorizontalLineSprite class
 * @version 0.1
 * @date 2024-01-25
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "sprite/HorizontalLineSprite.hpp"

HorizontalLineSprite::HorizontalLineSprite(std::shared_ptr<IPaintingProxy> paintingProxy)
	: SpriteBase(paintingProxy)
	, r{0}
{}

Size2d HorizontalLineSprite::getSize()
{
	Size2d res;
	res.w = paintingProxy->getPaintAreaSize().w - getX() - getR();
	res.h = 1;
	return res;
}

void HorizontalLineSprite::repaint(std::shared_ptr<ICanvas> canvas, Rect& invalidRect)
{
	Size2d sprSize = getSize();
	
	canvas->setStrokingColor(Color::white());
	canvas->setStrokeWidth(sprSize.h);

	canvas->strokeLine(getX(), getY(), sprSize.w, getY());
	
	invalidRect += getBounds();
}

int HorizontalLineSprite::getR() const
{
	return r;
}

void HorizontalLineSprite::setR(int value)
{
	r = value;
}
