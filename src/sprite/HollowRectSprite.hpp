/**
 * @file HollowRectSprite.hpp
 * @author Tomáš Ludrovan
 * @brief HollowRectSprite class
 * @version 0.1
 * @date 2024-02-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef HOLLOWRECTSPRITE_HPP
#define HOLLOWRECTSPRITE_HPP

#include "sprite/PositionedSpriteBase.hpp"

class HollowRectSprite : public PositionedSpriteBase {
private:
	Color m_color;
	int m_bigRectWidth;
	int m_bigRectHeight;
	Rect m_hole;
protected:
	void repaintAsVisible(std::shared_ptr<ICanvas> canvas,
		const Rect& invalidRect) override;
public:
	HollowRectSprite(std::shared_ptr<IPaintingProxy> paintingProxy);
	
	/**
	 * @brief Returns the size (width and height) of the sprite.
	 * 
	 * @details This should always yield the same result as calling the Size2d
	 *          constructor as `Size2d(getW(), getH())`. However, calling this
	 *          method should be preferred, because descendants may optimize it
	 *          to gain performance.
	 */
	Size2d getSize() override;

	const Color& getColor() const;
	void setColor(const Color& value);
	int getBigRectWidth() const;
	void setBigRectWidth(int value);
	int getBigRectHeight() const;
	void setBigRectHeight(int value);
	const Rect& getHole() const;
	void setHole(const Rect& value);
};

#endif // HOLLOWRECTSPRITE_HPP
