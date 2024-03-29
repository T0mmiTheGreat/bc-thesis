/**
 * @file TextBoxSprite.hpp
 * @author Tomáš Ludrovan
 * @brief TextBoxSprite class
 * @version 0.1
 * @date 2024-02-12
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef TEXTBOXSPRITE_HPP
#define TEXTBOXSPRITE_HPP

#include <memory>
#include <string>

#include "types.hpp"
#include "paintingproxy/IPaintingProxy.hpp"
#include "sprite/BoundedSpriteBase.hpp"

class TextBoxSprite : public BoundedSpriteBase {
private:
	Rect m_textRect;
	std::string m_text;
	FontId m_font;
	Color m_color;
protected:
	/**
	 * @brief Repaints an area of sprite.
	 * 
	 * @details This method is called from `repaint()` after it has been
	 *          observed that the "is visible" property is `true`.
	 * 
	 * @param canvas Canvas to paint onto.
	 * @param invalidRect Area to repaint.
	 * 
	 * @remark The sprite may choose to repaint area larger than invalidRect.
	 *         Everything outside will be clipped.
	 */
	void repaintAsVisible(std::shared_ptr<ICanvas> canvas,
		const Rect& invalidRect) override;
public:
	TextBoxSprite(std::shared_ptr<IPaintingProxy> paintingProxy);
	/**
	 * @brief Returns the rectangle that represents the bounds of the sprite.
	 */
	Rect getBounds() override;

	/**
	 * @brief Getter for the "text rect" property.
	 * 
	 * @details The text will be centered within this rectangle.
	 */
	const Rect& getTextRect() const;
	/**
	 * @brief Setter for the "text rect" property.
	 * 
	 * @details The text will be centered within this rectangle.
	 */
	void setTextRect(const Rect& value);
	/**
	 * @brief Getter for the "text" property.
	 */
	const std::string& getText() const;
	/**
	 * @brief Setter for the "text" property.
	 */
	void setText(const std::string& value);
	/**
	 * @brief Getter for the "font" property.
	 */
	FontId getFont() const;
	/**
	 * @brief Setter for the "font" property.
	 */
	void setFont(FontId value);
	/**
	 * @brief Getter for the "color" property.
	 */
	const Color& getColor() const;
	/**
	 * @brief Setter for the "color" property.
	 */
	void setColor(const Color& value);
};

#endif // TEXTBOXSPRITE_HPP
