/**
 * @file PositionSpriteBase.hpp
 * @author Tomáš Ludrovan
 * @brief PositionSpriteBase class
 * @version 0.1
 * @date 2024-01-31
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef POSITIONEDSPRITEBASE_HPP
#define POSITIONEDSPRITEBASE_HPP

#include <memory>

#include "types.hpp"
#include "paintingproxy/IPaintingProxy.hpp"
#include "sprite/BoundedSpriteBase.hpp"
#include "sprite/IPositionedSprite.hpp"

class PositionedSpriteBase : public IPositionedSprite, virtual public BoundedSpriteBase {
protected:
	int x;
	int y;

	/**
	 * @brief The sprite position has changed.
	 * 
	 * @details Descendants should call this method if they override it.
	 */
	virtual void posChangedEvent(int oldX, int oldY, int newX, int newY);
public:
	PositionedSpriteBase(std::shared_ptr<IPaintingProxy> paintingProxy);
	/**
	 * @brief Get the X coordinate of the sprite.
	 * 
	 * @details Returns the `x` value.
	 */
	virtual int getX() override;
	/**
	 * @brief Get the Y coordinate of the sprite.
	 * 
	 * @details Returns the `y` value.
	 */
	virtual int getY() override;
	/**
	 * @brief Get the width of the sprite.
	 * 
	 * @details Uses `getBounds()` to get the width.
	 */
	virtual int getW() override;
	/**
	 * @brief Get the height of the sprite.
	 * 
	 * @details Uses `getBounds()` to get the height.
	 */
	virtual int getH() override;
	/**
	 * @brief Set the X coordinate of the sprite.
	 * 
	 * @details Sets the `x` value. Causes screen invalidation.
	 */
	virtual void setX(int value) override;
	/**
	 * @brief Set the Y coordinate of the sprite.
	 * 
	 * @details Sets the `y` value. Causes screen invalidation.
	 */
	virtual void setY(int value) override;
	/**
	 * @brief Sets the X and Y coordinates of the sprite.
	 * 
	 * @details If both X and Y has to be set, this should be preferred over
	 *          calling setX() and setY() separately, although that should still
	 *          be possible.
	 * 
	 *          Sets both `x` and `y` value. Causes screen invalidation.
	 */
	virtual void setPos(int x, int y) override;
	/**
	 * @brief Sets the X and Y coordinates of the sprite.
	 * 
	 * @details If both X and Y has to be set, this should be preferred over
	 *          calling setX() and setY() separately, although that should still
	 *          be possible.
	 */
	virtual void setPos(Point p) override;
	/**
	 * @brief Returns the rectangle that represents the bounds of the sprite.
	 * 
	 * @details This should always yield the same result as calling the Rect
	 *          constructor as `Rect(getX(), getY(), getW(), getH())`. However,
	 *          calling this method should be preferred, because descendants may
	 *          optimize it to gain performance.
	 * 
	 *          Uses `getX()`, `getY()` and `getSize()` to get the bounds rect.
	 */
	virtual Rect getBounds() override;

	/**
	 * @brief Returns the size (width and height) of the sprite.
	 * 
	 * @details This should always yield the same result as calling the Size2d
	 *          constructor as `Size2d(getW(), getH())`. However, calling this
	 *          method should be preferred, because descendants may optimize it
	 *          to gain performance.
	 */
	virtual Size2d getSize() override = 0;
	/**
	 * @brief Repaints an area of sprite.
	 * 
	 * @param canvas Canvas to paint onto.
	 * @param invalidRect Area to repaint.
	 * 
	 * @remark The sprite may choose to repaint area larger than invalidRect,
	 *         but it must modify the invalidRect value to mach the area it
	 *         painted.
	 */
	virtual void repaint(std::shared_ptr<ICanvas> canvas, Rect& invalidRect) override = 0;
};

#endif // POSITIONEDSPRITEBASE_HPP
