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
	paintingProxy->invalidateRect(getBounds());
}

BoundedSpriteBase::BoundedSpriteBase(
	std::shared_ptr<IPaintingProxy> paintingProxy)
	: SpriteBase(paintingProxy)
{}
