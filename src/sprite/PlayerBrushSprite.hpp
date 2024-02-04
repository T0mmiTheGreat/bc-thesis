/**
 * @file PlayerBrushSprite.hpp
 * @author Tomáš Ludrovan
 * @brief PlayerBrushSprite class
 * @version 0.1
 * @date 2024-02-03
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef PLAYERBRUSHSPRITE_HPP
#define PLAYERBRUSHSPRITE_HPP

#include <memory>

#include "types.hpp"
#include "paintingproxy/IPaintingProxy.hpp"
#include "sprite/PositionedSpriteBase.hpp"

/**
 * @brief Used as player tool brush in stage editor.
 */
class PlayerBrushSprite : public PositionedSpriteBase {
public:
	enum Costume {
		COSTUME_NORMAL, // Player object may be placed
		COSTUME_BAD,    // Player object may not be placed
	};
private:
	static constexpr uint8_t SPRITE_ALPHA = 0xa4;
	static constexpr Color STROKE_NORMAL = Color::white().setAlpha(SPRITE_ALPHA);
	static constexpr Color STROKE_BAD = Color::red().setAlpha(SPRITE_ALPHA);

	Costume m_costume;
	int m_radius;

	void repaintNormal(std::shared_ptr<ICanvas> canvas, Rect& invalidRect);
	void repaintBad(std::shared_ptr<ICanvas> canvas, Rect& invalidRect);
public:
	PlayerBrushSprite(std::shared_ptr<IPaintingProxy> paintingProxy);
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
	void repaint(std::shared_ptr<ICanvas> canvas, Rect& invalidRect) override;

	/**
	 * @brief Getter for the sprite costume.
	 */
	PlayerBrushSprite::Costume getCostume() const;
	/**
	 * @brief Setter for the sprite costume.
	 */
	void setCostume(PlayerBrushSprite::Costume value);
	/**
	 * @brief Getter for the player radius.
	 */
	int getRadius() const;
	/**
	 * @brief Setter for the player radius.
	 */
	void setRadius(int value);

	/**
	 * @brief Returns the X coordinate of the sprite center.
	 */
	int getCenterX() const;
	/**
	 * @brief Returns the Y coordinate of the sprite center.
	 */
	int getCenterY() const;
	/**
	 * @brief Returns the position of the sprite center.
	 */
	Point getCenterPos() const;
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

#endif // PLAYERBRUSHSPRITE_HPP
