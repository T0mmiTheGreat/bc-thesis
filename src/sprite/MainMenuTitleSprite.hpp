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
#include "sprite/PositionedSpriteBase.hpp"

class MainMenuTitleSprite : public PositionedSpriteBase {
private:
	// The title text to show
	const std::string TITLE_TEXT = "BUBBLE BRAWL";
	// The font of the title text
	const FontId TITLE_FONT = FONT_JULEE_66;
protected:
	void repaintAsVisible(std::shared_ptr<ICanvas> canvas,
		const Rect& invalidRect) override;
public:
	MainMenuTitleSprite(std::shared_ptr<IPaintingProxy> paintingProxy);

	/**
	 * @brief Returns the size (width and height) of the sprite.
	 * 
	 * @details This should always yield the same result as calling the Size2d
	 *          constructor as `Size2d(getW(), getH())`. However, calling this
	 *          method should be preferred, because this object may optimize it
	 *          to gain performance.
	 */
	Size2d getSize() override;
};

#endif // MAINMENUTITLESPRITE_HPP
