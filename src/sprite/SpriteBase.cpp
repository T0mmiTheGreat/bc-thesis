/**
 * @file SpriteBase.cpp
 * @author Tomáš Ludrovan
 * @brief SpriteBase class
 * @version 0.1
 * @date 2023-12-19
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "sprite/SpriteBase.hpp"

void SpriteBase::invalidate()
{
	invalidateRect();
}

void SpriteBase::invalidateRect()
{
	paintingProxy->invalidateRect();
}

void SpriteBase::invalidateRect(const Rect& rect)
{
	paintingProxy->invalidateRect(rect);
}

SpriteBase::SpriteBase(std::shared_ptr<IPaintingProxy> paintingProxy)
	: paintingProxy{paintingProxy}
	, isVisible{true}
{}

SpriteBase::~SpriteBase()
{
	invalidate();
}

bool SpriteBase::getIsVisible()
{
	return isVisible;
}

void SpriteBase::setIsVisible(bool value)
{
	if (isVisible != value) {
		isVisible = value;
		invalidate();
	}
}

void SpriteBase::forceUpdate()
{
	invalidate();
}

void SpriteBase::repaint(std::shared_ptr<ICanvas> canvas,
	const Rect& invalidRect)
{
	if (isVisible) {
		repaintAsVisible(canvas, invalidRect);
	}
}
