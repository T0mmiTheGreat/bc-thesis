/**
 * @file BoundedSpriteBase.cpp
 * @author Tomáš Ludrovan
 * @brief BoundedSpriteBase class
 * @version 0.1
 * @date 2024-01-31
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "sprite/BoundedSpriteBase.hpp"

void BoundedSpriteBase::invalidateBounds()
{
	// Sprites are allowed off-by-one pixel errors
	Rect boundsRect = getBounds();
	boundsRect.x -= 1;
	boundsRect.y -= 1;
	boundsRect.w += 2;
	boundsRect.h += 2;

	paintingProxy->invalidateRect(boundsRect);
}

BoundedSpriteBase::BoundedSpriteBase(
	std::shared_ptr<IPaintingProxy> paintingProxy)
	: SpriteBase(paintingProxy)
{}
