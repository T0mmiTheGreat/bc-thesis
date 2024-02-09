/**
 * @file ISprite.hpp
 * @author Tomáš Ludrovan
 * @brief ISprite interface
 * @version 0.1
 * @date 2023-12-19
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef ISPRITE_HPP
#define ISPRITE_HPP

#include <memory>

#include "types.hpp"
#include "canvas/ICanvas.hpp"

/**
 * @brief Sprite.
 */
class ISprite {
public:
	virtual ~ISprite() {}
	/**
	 * @brief Getter for the "is visible" property.
	 * 
	 * @details This read-write property indicates whether the sprite is shown.
	 *          If the value is `false`, the sprite will not be painted when
	 *          repainting takes place.
	 */
	virtual bool getIsVisible() = 0;
	/**
	 * @brief Setter for the "is visible" property.
	 * 
	 * @copydetail ISprite::getIsVisible()
	 */
	virtual void setIsVisible(bool value) = 0;
	/**
	 * @brief Forces the sprite to update.
	 * 
	 * @details Calling this method will make the sprite the invalidate screen.
	 */
	virtual void forceUpdate() = 0;
	/**
	 * @brief Repaints an area of sprite.
	 * 
	 * @param canvas Canvas to paint onto.
	 * @param invalidRect Area to repaint.
	 * 
	 * @remark The sprite may choose to repaint area larger than invalidRect.
	 *         Everything outside will be clipped.
	 */
	virtual void repaint(std::shared_ptr<ICanvas> canvas, const Rect& invalidRect) = 0;
};

#endif // ISPRITE_HPP
