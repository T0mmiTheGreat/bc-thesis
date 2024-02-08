/**
 * @file SpriteBase.hpp
 * @author Tomáš Ludrovan
 * @brief SpriteBase class
 * @version 0.1
 * @date 2023-12-19
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef SPRITEBASE_HPP
#define SPRITEBASE_HPP

#include <memory>

#include "sprite/ISprite.hpp"
#include "paintingproxy/IPaintingProxy.hpp"

// TODO: Begin change + End change (to optimize painting)

/**
 * @brief Base class for all sprites.
 * 
 * @details Every descendant MUST provide implementation of getSize() and
 *          repaint() methods. They MUST call this class's constructor if
 *          they provide their own.
 */
class SpriteBase : virtual public ISprite {
protected:
	std::shared_ptr<IPaintingProxy> paintingProxy;
public:
	SpriteBase(std::shared_ptr<IPaintingProxy> paintingProxy);
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
	virtual void repaint(std::shared_ptr<ICanvas> canvas, const Rect& invalidRect) override = 0;
};

#endif // SPRITEBASE_HPP
