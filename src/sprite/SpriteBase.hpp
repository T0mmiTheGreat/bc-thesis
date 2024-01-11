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
 */
class SpriteBase : public ISprite {
protected:
	int x;
	int y;
	std::shared_ptr<IPaintingProxy> paintingProxy;
	bool isAnimationRunningFlag;

	/**
	 * @brief Calls `invalidateRect()` with the sprite bounds as the parameter.
	 * 
	 * @details `sysProxy->invalidateRect(getBounds())`.
	 */
	void invalidateBounds();
	/**
	 * @brief Represent the animation progress as a number in interval <0,1>.
	 * 
	 * @param frameNumber Current frame number.
	 * @param frameCount Maximum frame number.
	 */
	static constexpr double getAnimProgress(int frameNumber, unsigned frameCount) {
		return static_cast<double>(frameNumber) / static_cast<double>(frameCount - 1);
	}

	/**
	 * @brief The sprite position has changed.
	 */
	virtual void posChangedEvent(int oldX, int oldY, int newX, int newY);
public:
	SpriteBase(std::shared_ptr<IPaintingProxy> paintingProxy);
	/**
	 * @brief Get the X coordinate of the sprite.
	 * 
	 * @details Returns the `x` value.
	 */
	virtual int getX() override;
	/**
	 * @brief Get the Y coordinate of the sprite.
	 * 
	 * @details Returns the `y` value.
	 */
	virtual int getY() override;
	/**
	 * @brief Get the width of the sprite.
	 * 
	 * @details Uses `getBounds()` to get the width.
	 */
	virtual int getW() override;
	/**
	 * @brief Get the height of the sprite.
	 * 
	 * @details Uses `getBounds()` to get the height.
	 */
	virtual int getH() override;
	/**
	 * @brief Set the X coordinate of the sprite.
	 * 
	 * @details Sets the `x` value. Causes screen invalidation.
	 */
	virtual void setX(int value) override;
	/**
	 * @brief Set the Y coordinate of the sprite.
	 * 
	 * @details Sets the `y` value. Causes screen invalidation.
	 */
	virtual void setY(int value) override;
	/**
	 * @brief Sets the X and Y coordinates of the sprite.
	 * 
	 * @details If both X and Y has to be set, this should be preferred over
	 *          calling setX() and setY() separately, although that should still
	 *          be possible.
	 * 
	 *          Sets both `x` and `y` value. Causes screen invalidation.
	 */
	virtual void setPos(int x, int y) override;
	/**
	 * @brief Returns the rectangle that represents the bounds of the sprite.
	 * 
	 * @details This should always yield the same result as calling the Rect
	 *          constructor as `Rect(getX(), getY(), getW(), getH())`. However,
	 *          calling this method should be preferred, because descendants may
	 *          optimize it to gain performance.
	 * 
	 *          Uses `getX()`, `getY()` and `getSize()` to get the bounds rect.
	 */
	virtual Rect getBounds() override;
	/**
	 * @brief Starts a sprite animation.
	 * 
	 * @details If the method is called while an animation is running, the
	 *          running animation is stopped.
	 * 
	 *         	Sets the `isAnimationRunningFlag`.
	 * 
	 * @remarks If a sprite doesn't have an animation, this should be no-op.
	 * 
	 *          If a sprite has multiple animations, they might provide methods
	 *          to select which animation to start. The caller then should first
	 *          call the method which selects the animation and then call this
	 *          method.
	 */
	virtual void startAnimation() override;
	/**
	 * @brief Stops an animation if one is running.
	 * 
	 * @details Clears the `isAnimationRunningFlag`.
	 * 
	 * @remarks This should be no-op if a sprite doesn't have an animation or
	 *          if animation is not running.
	 * 
	 *          It is NOT necessary to call this method after the animation
	 *          finishes. If it would be, the object itself should call it.
	 */
	virtual void stopAnimation() override;
	/**
	 * @brief Checks whether an animation is running.
	 * 
	 * @details Reads the `isAnimationRunningFlag`.
	 * 
	 * @remark Mainly used for checking whether the animation has finished.
	 */
	virtual bool isAnimationRunning() override;
	/**
	 * @brief Event that happens every event loop iteration.
	 * 
	 * @details This method is mainly used to progress any running animations,
	 *          but in the future there might be more functionality to this, so
	 *          it is better to call it every iteration no matter the sprite
	 *          does or does not have an animation.
	 * 
	 *          No-op unless overriden.
	 */
	virtual void loopEvent() override;
};

#endif // SPRITEBASE_HPP
