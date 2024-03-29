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
public:
	enum Costume {
		COSTUME_NORMAL,
		COSTUME_HIGHLIGHTED,
	};
private:
	static constexpr Color HIGHLIGHT_COLOR = Color::white();

	Costume m_costume;
	Color m_color;
	PolygonF m_shape;

	void repaintCostumeNormal(std::shared_ptr<ICanvas> canvas,
		const Rect& invalidRect);
	void repaintCostumeHighlighted(std::shared_ptr<ICanvas> canvas,
		const Rect& invalidRect);
protected:
	void repaintAsVisible(std::shared_ptr<ICanvas> canvas,
		const Rect& invalidRect) override;
public:
	ObstacleSprite(std::shared_ptr<IPaintingProxy> paintingProxy);
	/**
	 * @brief Returns the rectangle that represents the bounds of the sprite.
	 */
	Rect getBounds() override;

	/**
	 * @brief Getter for the player color.
	 */
	Color getColor() const;
	/**
	 * @brief Setter for the player color.
	 */
	void setColor(const Color& value);
	const PolygonF& getShape() const;
	void setShape(const PolygonF& value);
	void setShape(PolygonF&& value);
	/**
	 * @brief Getter for the obstacle costume.
	 */
	ObstacleSprite::Costume getCostume() const;
	/**
	 * @brief Setter for the obstacle costume.
	 */
	void setCostume(ObstacleSprite::Costume value);
};

#endif // OBSTACLESPRITE_HPP
