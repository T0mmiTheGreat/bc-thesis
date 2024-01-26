/**
 * @file VerticalLineSprite.cpp
 * @author Tomáš Ludrovan
 * @brief VerticalLineSprite class
 * @version 0.1
 * @date 2024-01-25
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "sprite/VerticalLineSprite.hpp"

VerticalLineSprite::VerticalLineSprite(std::shared_ptr<IPaintingProxy> paintingProxy)
	: SpriteBase(paintingProxy)
	, bottomMargin{0}
{}

Size2d VerticalLineSprite::getSize()
{
	Size2d res;
	res.w = 1;
	res.h = paintingProxy->getPaintAreaSize().h - getY() - getBottomMargin();
	return res;
}

void VerticalLineSprite::repaint(std::shared_ptr<ICanvas> canvas, Rect & invalidRect)
{
	Size2d sprSize = getSize();
	
	canvas->setStrokingColor(Color::white());
	canvas->setStrokeWidth(sprSize.w);

	canvas->strokeLine(getX(), getY(), getX(), sprSize.h);
	
	invalidRect += getBounds();
}

int VerticalLineSprite::getBottomMargin() const
{
	return bottomMargin;
}

void VerticalLineSprite::setBottomMargin(int value)
{
	invalidateBounds();
	bottomMargin = value;
	invalidateBounds();
}
