/**
 * @file StatusbarTextSprite.hpp
 * @author Tomáš Ludrovan
 * @brief StatusbarTextSprite class
 * @version 0.1
 * @date 2024-01-26
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef STATUSBARTEXTSPRITE_HPP
#define STATUSBARTEXTSPRITE_HPP

#include <memory>
#include <string>

#include "types.hpp"
#include "paintingproxy/IPaintingProxy.hpp"
#include "sprite/PositionedSpriteBase.hpp"

class StatusbarTextSprite : public PositionedSpriteBase {
private:
	static constexpr FontId FONT = FONT_ROBOTO_16;

	std::string m_text;
protected:
	void repaintAsVisible(std::shared_ptr<ICanvas> canvas,
		const Rect& invalidRect) override;
public:
	StatusbarTextSprite(std::shared_ptr<IPaintingProxy> paintingProxy);
	
	/**
	 * @brief Returns the size (width and height) of the sprite.
	 * 
	 * @details This should always yield the same result as calling the Size2d
	 *          constructor as `Size2d(getW(), getH())`. However, calling this
	 *          method should be preferred, because descendants may optimize it
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
};

#endif // STATUSBARTEXTSPRITE_HPP
