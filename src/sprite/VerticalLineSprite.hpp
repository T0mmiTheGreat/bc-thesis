/**
 * @file VerticalLineSprite.hpp
 * @author Tomáš Ludrovan
 * @brief VerticalLineSprite class
 * @version 0.1
 * @date 2024-01-25
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef VERTICALLINESPRITE_HPP
#define VERTICALLINESPRITE_HPP

#include "sprite/SpriteBase.hpp"

class VerticalLineSprite : public SpriteBase {
private:
	int bottomMargin;
public:
	VerticalLineSprite(std::shared_ptr<IPaintingProxy> paintingProxy);
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
	 * @brief Getter for the bottom margin of the line.
	 */
	int getBottomMargin() const;
	/**
	 * @brief Setter for the bottom margin of the line.
	 */
	void setBottomMargin(int value);
};

#endif // VERTICALLINESPRITE_HPP
