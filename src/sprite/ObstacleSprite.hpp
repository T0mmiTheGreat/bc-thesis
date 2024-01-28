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
#include "sprite/SpriteBase.hpp"

class ObstacleSprite : public SpriteBase {
private:
	// Note: The `m_shape` position is irrelevant and can be changed as needed.
	PolygonF m_shape;
public:
	ObstacleSprite(std::shared_ptr<IPaintingProxy> paintingProxy,
		const PolygonF& shape);
	ObstacleSprite(std::shared_ptr<IPaintingProxy> paintingProxy,
		PolygonF&& shape);
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

	const PolygonF& getShape() const;
	void setShape(const PolygonF& value);
	void setShape(PolygonF&& value);
};

#endif // OBSTACLESPRITE_HPP
