/**
 * @file ImageSprite.cpp
 * @author Tomáš Ludrovan
 * @brief ImageSprite class
 * @version 0.1
 * @date 2024-01-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "sprite/ImageSprite.hpp"

Rect ImageSprite::calculateImgBounds(
	std::shared_ptr<IPaintingProxy> paintingProxy, ImageId img)
{
	Size2d imgSize = paintingProxy->getImageSize(img);
	return Rect(0, 0, imgSize);
}

ImageSprite::ImageSprite(std::shared_ptr<IPaintingProxy> paintingProxy,
	ImageId img)
	: ImageSprite(paintingProxy, img, calculateImgBounds(paintingProxy, img))
{}

ImageSprite::ImageSprite(std::shared_ptr<IPaintingProxy> paintingProxy,
	ImageId img, const Rect& imgBounds)
	: SpriteBase(paintingProxy)
	, BoundedSpriteBase(paintingProxy)
	, PositionedSpriteBase(paintingProxy)
	, m_img{img}
	, m_imgBounds{imgBounds}
{}

Size2d ImageSprite::getSize()
{
	return m_imgBounds.getSize();
}

void ImageSprite::repaintAsVisible(std::shared_ptr<ICanvas> canvas, const Rect& invalidRect)
{
	(void)invalidRect;

	Rect dstRect(x, y, m_imgBounds.getSize());
	canvas->copyImage(m_img, m_imgBounds, dstRect);
}

ImageId ImageSprite::getImg() const
{
	return m_img;
}
