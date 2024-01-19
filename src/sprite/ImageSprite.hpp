/**
 * @file ImageSprite.hpp
 * @author Tomáš Ludrovan
 * @brief ImageSprite class
 * @version 0.1
 * @date 2024-01-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef IMAGESPRITE_HPP
#define IMAGESPRITE_HPP

#include <memory>

#include "types.hpp"
#include "paintingproxy/IPaintingProxy.hpp"
#include "sprite/SpriteBase.hpp"

class ImageSprite : public SpriteBase {
private:
	ImageId m_img;
	Rect m_imgBounds;

	static Rect calculateImgBounds(
		std::shared_ptr<IPaintingProxy> paintingProxy, ImageId img);
public:
	ImageSprite(std::shared_ptr<IPaintingProxy> paintingProxy, ImageId img);
	ImageSprite(std::shared_ptr<IPaintingProxy> paintingProxy, ImageId img,
		const Rect& imgBounds);
	/**
	 * @brief Returns the size (width and height) of the sprite.
	 * 
	 * @details This should always yield the same result as calling the Size2d
	 *          constructor as `Size2d(getW(), getH())`. However, calling this
	 *          method should be preferred, because this object may optimize it
	 *          to gain performance.
	 */
	Size2d getSize() override;
	/**
	 * @brief Repaints an area of sprite.
	 * 
	 * @param canvas Canvas to paint onto.
	 * @param invalidRect Area to repaint.
	 */
	void repaint(std::shared_ptr<ICanvas> canvas, Rect& invalidRect) override;
};

#endif // IMAGESPRITE_HPP
