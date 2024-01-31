/**
 * @file BoundedSpriteBase.hpp
 * @author Tomáš Ludrovan
 * @brief BoundedSpriteBase class
 * @version 0.1
 * @date 2024-01-31
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef BOUNDEDSPRITEBASE_HPP
#define BOUNDEDSPRITEBASE_HPP

#include "types.hpp"
#include "sprite/SpriteBase.hpp"

/**
 * @brief A sprite with definite bounds.
 * 
 */
class BoundedSpriteBase : virtual public SpriteBase {
protected:
	/**
	 * @brief Returns the rectangle that represents the bounds of the sprite.
	 */
	virtual Rect getBounds() = 0;
	/**
	 * @brief Calls `invalidateRect()` with the sprite bounds as the parameter.
	 * 
	 * @details `sysProxy->invalidateRect(getBounds())`.
	 */
	void invalidateBounds();
public:
	BoundedSpriteBase(std::shared_ptr<IPaintingProxy> paintingProxy);
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

#endif // BOUNDEDSPRITEBASE_HPP
