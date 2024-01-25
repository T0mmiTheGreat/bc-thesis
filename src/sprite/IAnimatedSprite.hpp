/**
 * @file IAnimatedSprite.hpp
 * @author Tomáš Ludrovan
 * @brief IAnimatedSprite interface
 * @version 0.1
 * @date 2024-01-25
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef IANIMATEDSPRITE_HPP
#define IANIMATEDSPRITE_HPP

#include "sprite/ISprite.hpp"

/**
 * @brief Sprite with some animation.
 */
class IAnimatedSprite : public ISprite {
public:
	virtual ~IAnimatedSprite() {}
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
	 *       animation may or may not be stopped.
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
	 * @brief Event that happens every event loop iteration.
	 * 
	 * @details This method is used to progress any running animations.
	 */
	virtual void loopEvent() = 0;
};

#endif // IANIMATEDSPRITE_HPP
