/**
 * @file OptionBarSprite.hpp
 * @author Tomáš Ludrovan
 * @brief OptionBarSprite class
 * @version 0.1
 * @date 2024-01-30
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef OPTIONBARSPRITE_HPP
#define OPTIONBARSPRITE_HPP

#include <memory>

#include "paintingproxy/IPaintingProxy.hpp"
#include "sprite/PositionedSpriteBase.hpp"

class OptionBarSprite : public PositionedSpriteBase {
public:
	struct Borders {
		bool left: 1;
		bool top: 1;
		bool right: 1;
		bool bottom: 1;

		Borders(bool left, bool top, bool right, bool bottom)
			: left{left}
			, top{top}
			, right{right}
			, bottom{bottom}
		{}

		Borders()
			: Borders(true, true, true, true)
		{}

		constexpr bool operator== (const Borders& rhs) const {
			return this->left   == rhs.left
				&& this->top    == rhs.top
				&& this->right  == rhs.right
				&& this->bottom == rhs.bottom;
		}
		constexpr bool operator!= (const Borders& rhs) const {
			return !(*this == rhs);
		}
	};
private:
	int m_barWidth;
	int m_barHeight;

	Borders m_borders;

	Color m_fColor;
	Color m_sColor;
protected:
	void repaintAsVisible(std::shared_ptr<ICanvas> canvas,
		const Rect& invalidRect) override;
public:
	OptionBarSprite(std::shared_ptr<IPaintingProxy> paintingProxy);

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
	 * @brief Getter for the barWidth property.
	 * 
	 * @details Values <= 0 have different meaning -- they are the offset from
	 *          the right border of the screen.
	 */
	int getBarWidth() const;
	/**
	 * @brief Setter for the barWidth property.
	 * 
	 * @details Values <= 0 have different meaning -- they are the offset from
	 *          the right border of the screen.
	 */
	void setBarWidth(int value);
	/**
	 * @brief Getter for the barHeight property.
	 * 
	 * @details Values <= 0 have different meaning -- they are the offset from
	 *          the bottom border of the screen.
	 */
	int getBarHeight() const;
	/**
	 * @brief Setter for the barHeight property.
	 * 
	 * @details Values <= 0 have different meaning -- they are the offset from
	 *          the bottom border of the screen.
	 */
	void setBarHeight(int value);

	/**
	 * @brief Getter for the borders property.
	 * 
	 * @details The borders structure contains four boolean values for each
	 *          side of the bar. True value means that the border should be
	 *          painted.
	 */
	const OptionBarSprite::Borders& getBorders() const;
	/**
	 * @brief Setter for the borders property.
	 * 
	 * @details The borders structure contains four boolean values for each
	 *          side of the bar. True value means that the border should be
	 *          painted.
	 */
	void setBorders(const OptionBarSprite::Borders& value);
	/**
	 * @brief Setter for the borders property.
	 * 
	 * @details The borders structure contains four boolean values for each
	 *          side of the bar. True value means that the border should be
	 *          painted.
	 */
	void setBorders(OptionBarSprite::Borders&& value);
	/**
	 * @brief Setter for the borders property.
	 * 
	 * @param left Whether the left border should be painted.
	 * @param top Whether the top border should be painted.
	 * @param right Whether the right border should be painted.
	 * @param bottom Whether the bottom border should be painted.
	 */
	void setBorders(bool left, bool top, bool right, bool bottom);

	/**
	 * @brief Getter for the filling color of the bar.
	 */
	const Color& getFillingColor() const;
	/**
	 * @brief Setter for the filling color of the bar.
	 */
	void setFillingColor(const Color& value);
	/**
	 * @brief Getter for the stroking color of the bar.
	 */
	const Color& getStrokingColor() const;
	/**
	 * @brief Setter for the stroking color of the bar.
	 */
	void setStrokingColor(const Color& value);
};

#endif // OPTIONBARSPRITE_HPP
