/**
 * @file ISprite.hpp
 * @author Tomáš Ludrovan
 * @brief ISprite interface
 * @version 0.1
 * @date 2023-12-19
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef ISPRITE_HPP
#define ISPRITE_HPP

#include <memory>

#include "types.hpp"
#include "canvas/ICanvas.hpp"

/**
 * @brief Sprite.
 */
class ISprite {
public:
	virtual ~ISprite() {}
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
	virtual void repaint(std::shared_ptr<ICanvas> canvas, Rect& invalidRect) = 0;
};

#endif // ISPRITE_HPP
