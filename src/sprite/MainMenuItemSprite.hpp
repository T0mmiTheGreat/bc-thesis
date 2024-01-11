/**
 * @file MainMenuItemSprite.hpp
 * @author Tomáš Ludrovan
 * @brief MainMenuItemSprite class
 * @version 0.1
 * @date 2023-12-19
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef MAINMENUITEMSPRITE_HPP
#define MAINMENUITEMSPRITE_HPP

#include <ctime>
#include <string>

#include "sprite/SpriteBase.hpp"

/**
 * @brief Sprite for main menu items.
 */
class MainMenuItemSprite : public SpriteBase {
public:
	enum Costume {
		// Default appearance
		COSTUME_NORMAL,
		// When mouse is over the item
		COSTUME_HOVER,
	};
private:
	// FIXME: static constexpr

	// Non-scaled rectangle width
	const int RECT_WIDTH_NORMAL = 165;
	// Non-scaled rectangle height
	const int RECT_HEIGHT_NORMAL = 35;
	// Scaled rectangle width
	const int RECT_WIDTH_SCALED = 198;
	// Scaled rectangle height
	const int RECT_HEIGHT_SCALED = 42;

	const FontId FONT = FONT_BRLNSTR_20;

	std::string m_text;
	Costume m_costume;
public:
	MainMenuItemSprite(std::shared_ptr<IPaintingProxy> paintingProxy);

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
	 * @brief Getter for the `m_text`.
	 */
	const std::string& getText() const;
	/**
	 * @brief Setter for the `m_text`.
	 */
	void setText(const std::string& value);
	
	/**
	 * @brief Getter for the `m_costume`.
	 */
	Costume getCostume() const;
	/**
	 * @brief Setter for the `m_costume`.
	 */
	void setCostume(Costume costume);
};

#endif // MAINMENUITEMSPRITE_HPP
