/**
 * @file ObstacleEdgesSprite.hpp
 * @author Tomáš Ludrovan
 * @brief ObstacleEdgesSprite class
 * @version 0.1
 * @date 2024-01-31
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef OBSTACLEEDGESSPRITE_HPP
#define OBSTACLEEDGESSPRITE_HPP

#include <memory>
#include <vector>

#include "types.hpp"
#include "paintingproxy/IPaintingProxy.hpp"
#include "sprite/BoundedSpriteBase.hpp"

class ObstacleEdgesSprite : public BoundedSpriteBase {
private:
	static constexpr Color STROKE_COLOR = Color::obstacle();

	std::vector<PointF> m_corners;
protected:
	void repaintAsVisible(std::shared_ptr<ICanvas> canvas,
		const Rect& invalidRect) override;
public:
	ObstacleEdgesSprite(std::shared_ptr<IPaintingProxy> paintingProxy);

	/**
	 * @brief Returns the rectangle that represents the bounds of the sprite.
	 */
	Rect getBounds() override;

	void setCorners(const std::vector<PointF>& value);
	void setCorners(std::vector<PointF>&& value);
	void clearCorners();
};

#endif // OBSTACLEEDGESSPRITE_HPP
