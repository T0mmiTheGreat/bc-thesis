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

class ISprite {
public:
	virtual ~ISprite() {}
	/**
	 * @brief Returns the X coordinate of the sprite.
	 */
	virtual int getX() = 0;
	/**
	 * @brief Returns the Y coordiante of the sprite.
	 */
	virtual int getY() = 0;
	/**
	 * @brief Returns the sprite width.
	 */
	virtual int getW() = 0;
	/**
	 * @brief Returns the sprite height.
	 */
	virtual int getH() = 0;
	/**
	 * @brief Sets the X coordinate of the sprite.
	 */
	virtual void setX(int value) = 0;
	/**
	 * @brief Sets the Y coordinate of the sprite.
	 */
	virtual void setY(int value) = 0;
	/**
	 * @brief Sets the X and Y coordinates of the sprite.
	 * 
	 * @details If both X and Y has to be set, this should be preferred over
	 *          calling setX() and setY() separately, although that should still
	 *          be possible.
	 */
	virtual void setPos(int x, int y) = 0;
	/**
	 * @brief Returns the rectangle that represents the bounds of the sprite.
	 * 
	 * @details This should always yield the same result as calling the Rect
	 *          constructor as `Rect(getX(), getY(), getW(), getH())`. However,
	 *          calling this method should be preferred, because descendants may
	 *          optimize it to gain performance.
	 */
	virtual Rect getBounds() = 0;
	/**
	 * @brief Repaint an area of sprite.
	 * 
	 * @param canvas Canvas to paint onto.
	 * @param invalidRect Area to repaint.
	 * 
	 * @remark The sprite may choose to repaint area larger than invalidRect,
	 *         but it must modify the invalidRect value to mach the area it
	 *         painted.
	 */
	virtual void repaint(std::shared_ptr<ICanvas> canvas, Rect& invalidRect) = 0;
};

#endif // ISPRITE_HPP
