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
#include "sprite/SpriteBase.hpp"

class StatusbarTextSprite : public SpriteBase {
private:
	static constexpr FontId FONT = FONT_TAHOMA_16;

	std::string m_text;
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
	 * @brief Repaints an area of sprite.
	 * 
	 * @param canvas Canvas to paint onto.
	 * @param invalidRect Area to repaint.
	 * 
	 * @remark The sprite may choose to repaint area larger than invalidRect,
	 *         but it must modify the invalidRect value to mach the area it
	 *         painted.
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
};

#endif // STATUSBARTEXTSPRITE_HPP
