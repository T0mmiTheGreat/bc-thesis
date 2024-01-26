/**
 * @file EditorIconSprite.hpp
 * @author Tomáš Ludrovan
 * @brief EditorIconSprite class
 * @version 0.1
 * @date 2024-01-26
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef EDITORICONSPRITE_HPP
#define EDITORICONSPRITE_HPP

#include "sprite/SpriteBase.hpp"

class EditorIconSprite : public SpriteBase {
public:
	enum Costume {
		COSTUME_NORMAL,   // Normal, unmodified
		COSTUME_HOVER,    // Mouse hover
		COSTUME_SELECTED, // Selected, active
		COSTUME_DISABLED, // Cannot be pressed
	};
private:
	static constexpr int IMG_MARGINS = 2;

	ImageId m_img;
	Costume m_costume;

	void paintImg(std::shared_ptr<ICanvas> canvas, Rect& invalidRect);
	void repaintCostumeNormal(std::shared_ptr<ICanvas> canvas, Rect& invalidRect);
	void repaintCostumeHover(std::shared_ptr<ICanvas> canvas, Rect& invalidRect);
	void repaintCostumeSelected(std::shared_ptr<ICanvas> canvas, Rect& invalidRect);
	void repaintCostumeDisabled(std::shared_ptr<ICanvas> canvas, Rect& invalidRect);
public:
	// For convenience, this sprite has fixed width and height (i.e., doesn't
	// depend on the actual image dimensions)

	static constexpr int FIXED_IMG_WIDTH = 32;
	static constexpr int FIXED_IMG_HEIGHT = FIXED_IMG_WIDTH;
	static constexpr int FIXED_WIDTH = FIXED_IMG_WIDTH + 2*IMG_MARGINS;
	static constexpr int FIXED_HEIGHT = FIXED_IMG_HEIGHT + 2*IMG_MARGINS;

	EditorIconSprite(std::shared_ptr<IPaintingProxy> paintingProxy,
		ImageId img);

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
	 * @brief Getter for the `m_costume`.
	 */
	Costume getCostume() const;
	/**
	 * @brief Setter for the `m_costume`.
	 */
	void setCostume(Costume costume);
};

#endif // EDITORICONSPRITE_HPP
