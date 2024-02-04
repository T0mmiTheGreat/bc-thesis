/**
 * @file IPositionedSprite.hpp
 * @author Tomáš Ludrovan
 * @brief IPositionedSprite interface
 * @version 0.1
 * @date 2024-01-31
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef IPOSITIONEDSPRITE_HPP
#define IPOSITIONEDSPRITE_HPP

#include "sprite/ISprite.hpp"

/**
 * @brief Sprite with trivial position.
 * 
 * @details "Trivial position" means that the sprite bounds are determined by
 *          X and Y coordinate, width, and height. When painting takes place,
 *          only area within these bounds must be painted. The said X and Y
 *          coordinates must be the top left corner of the bounds.
 */
class IPositionedSprite : virtual public ISprite {
public:
	virtual ~IPositionedSprite() {}
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
	 * @brief Sets the X and Y coordinates of the sprite.
	 * 
	 * @details If both X and Y has to be set, this should be preferred over
	 *          calling setX() and setY() separately, although that should still
	 *          be possible.
	 */
	virtual void setPos(Point p) = 0;
	/**
	 * @brief Returns the size (width and height) of the sprite.
	 * 
	 * @details This should always yield the same result as calling the Size2d
	 *          constructor as `Size2d(getW(), getH())`. However, calling this
	 *          method should be preferred, because descendants may optimize it
	 *          to gain performance.
	 */
	virtual Size2d getSize() = 0;
	/**
	 * @brief Returns the rectangle that represents the bounds of the sprite.
	 * 
	 * @details This should always yield the same result as calling the Rect
	 *          constructor as `Rect(getX(), getY(), getW(), getH())`. However,
	 *          calling this method should be preferred, because descendants may
	 *          optimize it to gain performance.
	 */
	virtual Rect getBounds() = 0;
};

#endif // IPOSITIONEDSPRITE_HPP
