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

class ISprite {
public:
	virtual ~ISprite() {}
	/**
	 * @brief Returns the X coordinate of the sprite.
	 */
	virtual int getX() = 0;
	/**
	 * @brief Returns the Y coordiante of the sprite.
	 */
	virtual int getY() = 0;
	/**
	 * @brief Returns the sprite width.
	 */
	virtual int getW() = 0;
	/**
	 * @brief Returns the sprite height.
	 */
	virtual int getH() = 0;
	/**
	 * @brief Sets the X coordinate of the sprite.
	 */
	virtual void setX(int value) = 0;
	/**
	 * @brief Sets the Y coordinate of the sprite.
	 */
	virtual void setY(int value) = 0;
	/**
	 * @brief Sets the X and Y coordinates of the sprite.
	 * 
	 * @details If both X and Y has to be set, this should be preferred over
	 *          calling setX() and setY() separately, although that should still
	 *          be possible.
	 */
	virtual void setPos(int x, int y) = 0;
	virtual Size2d getSize() = 0;
	/**
	 * @brief Returns the rectangle that represents the bounds of the sprite.
	 * 
	 * @details This should always yield the same result as calling the Rect
	 *          constructor as `Rect(getX(), getY(), getW(), getH())`. However,
	 *          calling this method should be preferred, because descendants may
	 *          optimize it to gain performance.
	 */
	virtual Rect getBounds() = 0;
	/**
	 * @brief Starts a sprite animation.
	 * 
	 * @remarks If a sprite doesn't have an animation, this should be no-op.
	 * 
	 *          If a sprite has multiple animations, they might provide methods
	 *          to select which animation to start. The caller then should first
	 *          call the method which selects the animation and then call this
	 *          method.
	 * 
	 * @note If the method is called while an animation is running, the running
	 *       animation is stopped.
	 */
	virtual void startAnimation() = 0;
	/**
	 * @brief Stops an animation if one is running.
	 * 
	 * @remarks This should be no-op if a sprite doesn't have an animation or
	 *          if animation is not running.
	 * 
	 *          It is NOT necessary to call this method after the animation
	 *          finishes. If it would be, the object itself should call it.
	 */
	virtual void stopAnimation() = 0;
	/**
	 * @brief Checks whether an animation is running.
	 * 
	 * @remark Mainly used for checking whether the animation has finished.
	 */
	virtual bool isAnimationRunning() = 0;
	/**
	 * @brief Event that happens every frame.
	 * 
	 * @details This method is mainly used to progress any running animations,
	 *          but in the future there might be more functionality to this, so
	 *          it is better to call it every frame no matter the sprite does
	 *          or does not have an animation.
	 */
	virtual void frameEvent() = 0;
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
