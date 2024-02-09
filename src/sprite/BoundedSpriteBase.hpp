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
	 * @brief Invalidates the area taken by the sprite.
	 */
	virtual void invalidate() override;
	/**
	 * @brief Repaints an area of sprite.
	 * 
	 * @details This method is called from `repaint()` after it has been
	 *          observed that the "is visible" property is `true`.
	 * 
	 * @param canvas Canvas to paint onto.
	 * @param invalidRect Area to repaint.
	 * 
	 * @remark The sprite may choose to repaint area larger than invalidRect.
	 *         Everything outside will be clipped.
	 */
	virtual void repaintAsVisible(std::shared_ptr<ICanvas> canvas,
		const Rect& invalidRect) override = 0;
public:
	BoundedSpriteBase(std::shared_ptr<IPaintingProxy> paintingProxy);
	/**
	 * @brief Returns the rectangle that represents the bounds of the sprite.
	 */
	virtual Rect getBounds() = 0;
};

#endif // BOUNDEDSPRITEBASE_HPP
