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
#include "sprite/SpriteBase.hpp"

class EditorWorkspaceGridSprite : public SpriteBase {
private:
	static constexpr Color GRID_COLOR = Color::grayscale(0x4a);

	double m_xSpacing;
	double m_ySpacing;
	Size2d m_size;
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
