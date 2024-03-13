/**
 * @file BonusSprite.hpp
 * @author Tomáš Ludrovan
 * @brief BonusSprite class
 * @version 0.1
 * @date 2024-03-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef BONUSSPRITE_HPP
#define BONUSSPRITE_HPP

#include "sprite/PositionedSpriteBase.hpp"

class BonusSprite : public PositionedSpriteBase {
private:
	static constexpr ImageId IMAGE_ID = IMG_BONUS_HP;

	int m_radius;
protected:
	void repaintAsVisible(std::shared_ptr<ICanvas> canvas,
		const Rect& invalidRect) override;
public:
	BonusSprite(std::shared_ptr<IPaintingProxy> paintingProxy);
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
	 * @brief Getter for the bonus sprite radius.
	 */
	int getRadius() const;
	/**
	 * @brief Setter for the bonus sprite radius.
	 */
	void setRadius(int value);

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

#endif // BONUSSPRITE_HPP
