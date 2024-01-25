/**
 * @file AnimatedSpriteBase.hpp
 * @author Tomáš Ludrovan
 * @brief AnimatedSpriteBase class
 * @version 0.1
 * @date 2024-01-25
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef ANIMATEDSPRITEBASE_HPP
#define ANIMATEDSPRITEBASE_HPP

#include <memory>

#include "sprite/IAnimatedSprite.hpp"
#include "sprite/SpriteBase.hpp"
#include "paintingproxy/IPaintingProxy.hpp"

/**
 * @brief Base class for all animated sprites.
 * 
 * @details Descendants MUST provide implementation for frameEvent() method.
 *          They MUST call this class's constructor if they provide their own.
 */
class AnimatedSpriteBase
	: public IAnimatedSprite
	, public SpriteBase
{
private:
	bool isAnimationRunningFlag;
public:
	AnimatedSpriteBase(std::shared_ptr<IPaintingProxy> paintingProxy);
	/**
	 * @brief Get the X coordinate of the sprite.
	 * 
	 * @details Returns the `x` value.
	 */
	virtual int getX() override { return SpriteBase::getX(); }
	/**
	 * @brief Get the Y coordinate of the sprite.
	 * 
	 * @details Returns the `y` value.
	 */
	virtual int getY() override { return SpriteBase::getY(); }
	/**
	 * @brief Get the width of the sprite.
	 * 
	 * @details Uses `getBounds()` to get the width.
	 */
	virtual int getW() override { return SpriteBase::getW(); }
	/**
	 * @brief Get the height of the sprite.
	 * 
	 * @details Uses `getBounds()` to get the height.
	 */
	virtual int getH() override { return SpriteBase::getH(); }
	/**
	 * @brief Set the X coordinate of the sprite.
	 * 
	 * @details Sets the `x` value. Causes screen invalidation.
	 */
	virtual void setX(int value) override { return SpriteBase::setX(value); }
	/**
	 * @brief Set the Y coordinate of the sprite.
	 * 
	 * @details Sets the `y` value. Causes screen invalidation.
	 */
	virtual void setY(int value) override { return SpriteBase::setY(value); }
	/**
	 * @brief Sets the X and Y coordinates of the sprite.
	 * 
	 * @details If both X and Y has to be set, this should be preferred over
	 *          calling setX() and setY() separately, although that should still
	 *          be possible.
	 * 
	 *          Sets both `x` and `y` value. Causes screen invalidation.
	 */
	virtual void setPos(int x, int y) override { return SpriteBase::setPos(x, y); }
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
	virtual Rect getBounds() override { return SpriteBase::getBounds(); }
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
};

#endif // ANIMATEDSPRITEBASE_HPP
