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
	: virtual public IAnimatedSprite
	, virtual public SpriteBase
{
private:
	bool isAnimationRunningFlag;
protected:
	/**
	 * @brief Repaints an area of sprite.
	 * 
	 * @details This method is called from `repaint()` after it has been
	 *          observed that the "is visible" property is `true`.
	 * 
	 * @param canvas Canvas to paint onto.
	 * @param invalidRect Area to repaint.
	 * 
	 * @remark The sprite may choose to repaint area larger than invalidRect.
	 *         Everything outside will be clipped.
	 */
	virtual void repaintAsVisible(std::shared_ptr<ICanvas> canvas,
		const Rect& invalidRect) override = 0;
public:
	AnimatedSpriteBase(std::shared_ptr<IPaintingProxy> paintingProxy);
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
