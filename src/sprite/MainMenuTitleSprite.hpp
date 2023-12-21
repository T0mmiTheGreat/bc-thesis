/**
 * @file MainMenuTitleSprite.hpp
 * @author Tomáš Ludrovan
 * @brief MainMenuTitleSprite class
 * @version 0.1
 * @date 2023-12-20
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef MAINMENUTITLESPRITE_HPP
#define MAINMENUTITLESPRITE_HPP

#include <string>

#include "types.hpp"
#include "sprite/SpriteBase.hpp"

class MainMenuTitleSprite : public SpriteBase {
private:
	const std::string TITLE_TEXT = "MAIN TITLE";
	const FontId TITLE_FONT = FONT_SEGOEPR_66;
public:
	MainMenuTitleSprite();

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
};

#endif // MAINMENUTITLESPRITE_HPP
