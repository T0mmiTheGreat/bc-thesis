/**
 * @file ObstacleSprite.hpp
 * @author Tomáš Ludrovan
 * @brief ObstacleSprite class
 * @version 0.1
 * @date 2024-01-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef OBSTACLESPRITE_HPP
#define OBSTACLESPRITE_HPP

#include <memory>

#include "types.hpp"
#include "paintingproxy/IPaintingProxy.hpp"
#include "sprite/BoundedSpriteBase.hpp"

class ObstacleSprite : public BoundedSpriteBase {
private:
	PolygonF m_shape;
public:
	ObstacleSprite(std::shared_ptr<IPaintingProxy> paintingProxy);
	/**
	 * @brief Returns the rectangle that represents the bounds of the sprite.
	 */
	Rect getBounds() override;
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
	void repaint(std::shared_ptr<ICanvas> canvas, Rect& invalidRect) override;

	const PolygonF& getShape() const;
	void setShape(const PolygonF& value);
	void setShape(PolygonF&& value);
};

#endif // OBSTACLESPRITE_HPP
