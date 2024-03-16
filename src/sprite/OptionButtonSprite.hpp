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
	static constexpr FontId FONT = FONT_BRLNSTR_20;

	std::string m_text;
	Costume m_costume;
	Size2d m_size;

	Size2d getSizeUnscaled() const;
protected:
	void repaintAsVisible(std::shared_ptr<ICanvas> canvas,
		const Rect& invalidRect) override;
public:
	static constexpr Size2d BUTTON_SIZE_MENU = Size2d(198, 42);
	static constexpr Size2d BUTTON_SIZE_NARROWED = Size2d(140,
		BUTTON_SIZE_MENU.h);

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
	 * @brief Setter for the `m_size`.
	 */
	void setSize(const Size2d& value);

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
