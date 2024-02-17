/**
 * @file StageItemSprite.hpp
 * @author Tomáš Ludrovan
 * @brief StageItemSprite class
 * @version 0.1
 * @date 2024-02-17
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef STAGEITEMSPRITE_HPP
#define STAGEITEMSPRITE_HPP

#include <memory>
#include <string>

#include "types.hpp"
#include "paintingproxy/IPaintingProxy.hpp"
#include "sprite/PositionedSpriteBase.hpp"

class StageItemSprite : public PositionedSpriteBase {
public:
	enum Costume {
		COSTUME_NORMAL,
		COSTUME_HOVER,
	};
private:
	static constexpr int PADDING_VERT = 12;
	static constexpr int PADDING_LEFT = PADDING_VERT;
	static constexpr int TEXT_SPACING = 4;
	static constexpr FontId STAGE_TITLE_FONT = FONT_TAHOMA_20;
	static constexpr FontId STAGE_PATH_FONT = FONT_TAHOMA_16;
	static constexpr Color STAGE_TITLE_TEXT_COLOR = Color::white();
	static constexpr uint8_t STAGE_PATH_TEXT_ALPHA = 0x88;
	static constexpr Color STAGE_PATH_TEXT_COLOR = Color::white()
		.setAlpha(STAGE_PATH_TEXT_ALPHA);
	static constexpr Color BORDER_COLOR_HOVER = Color::white();
	static constexpr uint8_t BORDER_ALPHA_NORMAL = 0x66;
	static constexpr Color BORDER_COLOR_NORMAL = Color(BORDER_COLOR_HOVER)
		.setAlpha(BORDER_ALPHA_NORMAL);

	Costume m_costume;
	std::string m_stageTitle;
	std::string m_stagePath;
	int m_borderWidth;

	void repaintColor(std::shared_ptr<ICanvas> canvas,
		const Rect& invalidRect, const Color& borderColor);
	void repaintNormal(std::shared_ptr<ICanvas> canvas,
		const Rect& invalidRect);
	void repaintHover(std::shared_ptr<ICanvas> canvas,
		const Rect& invalidRect);
protected:
	void repaintAsVisible(std::shared_ptr<ICanvas> canvas,
		const Rect& invalidRect) override;
public:
	static constexpr int BORDER_HEIGHT = PADDING_VERT + 20 + TEXT_SPACING + 16
		+ PADDING_VERT;

	StageItemSprite(std::shared_ptr<IPaintingProxy> paintingProxy);
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
	 * @brief Getter for the "costume" property.
	 */
	StageItemSprite::Costume getCostume() const;
	/**
	 * @brief Setter for the "costume" property.
	 */
	void setCostume(StageItemSprite::Costume value);
	/**
	 * @brief Getter for the "stage title" property.
	 */
	const std::string& getStageTitle() const;
	/**
	 * @brief Setter for the "stage title" property.
	 */
	void setStageTitle(const std::string& value);
	/**
	 * @brief Getter for the "stage path" property.
	 */
	const std::string& getStagePath() const;
	/**
	 * @brief Setter for the "stage path" property.
	 */
	void setStagePath(const std::string& value);
	/**
	 * @brief Getter for the "border width" property.
	 * 
	 * @details The item has a border (a rectangle). The height of the border
	 *          is set automatically based on the text. The width, however, can
	 *          be adjusted. This affects only the bounds, so if the text is too
	 *          long, it will overflow the border.
	 */
	int getBorderWidth() const;
	/**
	 * @brief Setter for the "border width" property.
	 * 
	 * @copydetail StageItemSprite::getBorderWidth()
	 */
	void setBorderWidth(int value);
};

#endif // STAGEITEMSPRITE_HPP
