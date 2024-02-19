/**
 * @file OptionButtonSprite.hpp
 * @author Tomáš Ludrovan
 * @brief OptionButtonSprite class
 * @version 0.1
 * @date 2024-02-12
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef OPTIONBUTTONSPRITE_HPP
#define OPTIONBUTTONSPRITE_HPP


#include <string>

#include "sprite/PositionedSpriteBase.hpp"

class OptionButtonSprite : public PositionedSpriteBase {
public:
	enum Costume {
		// Default appearance
		COSTUME_NORMAL,
		// When mouse is over the item
		COSTUME_HOVER,
	};
private:
	// Non-scaled rectangle width
	static constexpr int RECT_WIDTH_NORMAL = 165;
	// Non-scaled rectangle height
	static constexpr int RECT_HEIGHT_NORMAL = 35;
	// Scaled rectangle width
	static constexpr int RECT_WIDTH_SCALED = 198;
	// Scaled rectangle height
	static constexpr int RECT_HEIGHT_SCALED = 42;

	const FontId FONT = FONT_BRLNSTR_20;

	std::string m_text;
	Costume m_costume;
protected:
	void repaintAsVisible(std::shared_ptr<ICanvas> canvas,
		const Rect& invalidRect) override;
public:
	static constexpr int BUTTON_WIDTH = RECT_WIDTH_SCALED;
	static constexpr int BUTTON_HEIGHT = RECT_HEIGHT_SCALED;

	OptionButtonSprite(std::shared_ptr<IPaintingProxy> paintingProxy);

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
	void setCostume(Costume value);
};

#endif // OPTIONBUTTONSPRITE_HPP
