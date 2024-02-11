/**
 * @file EditorWorkspaceGridSprite.hpp
 * @author Tomáš Ludrovan
 * @brief EditorWorkspaceGridSprite class
 * @version 0.1
 * @date 2024-01-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef EDITORWORKSPACEGRIDSPRITE_HPP
#define EDITORWORKSPACEGRIDSPRITE_HPP

#include <memory>
#include <unordered_set>

#include "types.hpp"
#include "paintingproxy/IPaintingProxy.hpp"
#include "sprite/PositionedSpriteBase.hpp"

class EditorWorkspaceGridSprite : public PositionedSpriteBase {
public:
	enum Costume {
		COSTUME_NORMAL, // Stage can be resized
		COSTUME_BAD,    // Stage can not be resized
	};
private:
	static constexpr uint8_t GRID_CELL_ALPHA = 0x60;
	static constexpr Color GRID_BORDER_COLOR_NORMAL = Color::grayscale(0x4a);
	static constexpr Color GRID_CELL_COLOR_NORMAL =
		Color(GRID_BORDER_COLOR_NORMAL).setAlpha(GRID_CELL_ALPHA);
	static constexpr Color GRID_BORDER_COLOR_BAD = Color::bad();
	static constexpr Color GRID_CELL_COLOR_BAD = Color(GRID_BORDER_COLOR_BAD)
		.setAlpha(GRID_CELL_ALPHA);

	Costume m_costume;
	unsigned m_solidsFrequency;
	double m_xSpacing;
	double m_ySpacing;
	Size2d m_size;

	void repaintCostumeNormal(std::shared_ptr<ICanvas> canvas,
		const Rect& invalidRect);
	void repaintCostumeBad(std::shared_ptr<ICanvas> canvas,
		const Rect& invalidRect);
	void repaintColor(const Color& gridBorderColor, const Color& gridCellColor,
		std::shared_ptr<ICanvas> canvas, const Rect& invalidRect);
protected:
	void repaintAsVisible(std::shared_ptr<ICanvas> canvas,
		const Rect& invalidRect) override;
public:
	EditorWorkspaceGridSprite(std::shared_ptr<IPaintingProxy> paintingProxy);

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
	 * @brief Getter for the grid costume.
	 */
	EditorWorkspaceGridSprite::Costume getCostume() const;
	/**
	 * @brief Setter for the grid costume.
	 */
	void setCostume(EditorWorkspaceGridSprite::Costume value);
	/**
	 * @brief Getter for the solids frequency.
	 * 
	 * @details Within the grid the cell borders are mostly painted as dashed
	 *          lines with periodically occuring solid lines. The "solids
	 *          frequency" indicates how often the solid lines should occur.
	 *          Value 1 means every line will be solid, 2 means every other,
	 *          etc. To paint only dashed lines, set the value to 0.
	 */
	unsigned getSolidsFrequency() const;
	/**
	 * @brief Setter for the solids frequency.
	 * 
	 * @copydetail EditorWorkspaceGridSprite::getSolidsFrequency()
	 */
	void setSolidsFrequency(unsigned value);
	void setSize(const Size2d& value);
	void setSize(Size2d&& value);
	double getXSpacing() const;
	/**
	 * @brief Sets the space between vertical lines.
	 * 
	 * @param value The spacing, or 0.0 to disable vertical lines.
	 */
	void setXSpacing(double value);
	double getYSpacing() const;
	/**
	 * @brief Sets the space between horizontal lines.
	 * 
	 * @param value The spacing, or 0.0 to disable horizontal lines.
	 */
	void setYSpacing(double value);
};

#endif // EDITORWORKSPACEGRIDSPRITE_HPP
