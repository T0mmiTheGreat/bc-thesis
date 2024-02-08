/**
 * @file PlayerSprite.hpp
 * @author Tomáš Ludrovan
 * @brief PlayerSprite class
 * @version 0.1
 * @date 2024-01-13
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef PLAYERSPRITE_HPP
#define PLAYERSPRITE_HPP

#include "types.hpp"
#include "sprite/PositionedSpriteBase.hpp"

class PlayerSprite : public PositionedSpriteBase {
public:
	enum Costume {
		COSTUME_NORMAL,
		COSTUME_HIGHLIGHTED,
	};
private:
	static constexpr Color HIGHLIGHT_COLOR = Color::white();

	Costume m_costume;
	Color m_color;
	int m_radius;

	void repaintCostumeNormal(std::shared_ptr<ICanvas> canvas,
		const Rect& invalidRect);
	void repaintCostumeHighlighted(std::shared_ptr<ICanvas> canvas,
		const Rect& invalidRect);
public:
	PlayerSprite(std::shared_ptr<IPaintingProxy> paintingProxy);
	/**
	 * @brief Returns the size (width and height) of the sprite.
	 * 
	 * @details This should always yield the same result as calling the Size2d
	 *          constructor as `Size2d(getW(), getH())`. However, calling this
	 *          method should be preferred, because this object may optimize it
	 *          to gain performance.
	 */
	Size2d getSize() override;
	/**
	 * @brief Repaints an area of sprite.
	 * 
	 * @param canvas Canvas to paint onto.
	 * @param invalidRect Area to repaint.
	 */
	void repaint(std::shared_ptr<ICanvas> canvas, const Rect& invalidRect) override;

	/**
	 * @brief Getter for the player color.
	 */
	Color getColor() const;
	/**
	 * @brief Setter for the player color.
	 */
	void setColor(const Color& value);
	/**
	 * @brief Getter for the player radius.
	 */
	int getRadius() const;
	/**
	 * @brief Setter for the player radius.
	 */
	void setRadius(int value);
	/**
	 * @brief Getter for the player costume.
	 */
	PlayerSprite::Costume getCostume() const;
	/**
	 * @brief Setter for the player costume.
	 */
	void setCostume(PlayerSprite::Costume value);

	/**
	 * @brief Modifies the sprite X coordinate by setting the X of its center.
	 */
	void setCenterX(int value);
	/**
	 * @brief Modifies the sprite Y coordinate by setting the Y of its center.
	 */
	void setCenterY(int value);
	/**
	 * @brief Modifies the sprite position by setting the position of its center.
	 */
	void setCenterPos(int x, int y);
	/**
	 * @brief Modifies the sprite position by setting the position of its center.
	 */
	void setCenterPos(const Point& pos);
};

#endif // PLAYERSPRITE_HPP
