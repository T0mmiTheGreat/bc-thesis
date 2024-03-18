/**
 * @file LogoSprite.hpp
 * @author Tomáš Ludrovan
 * @brief LogoSprite class
 * @version 0.1
 * @date 2023-12-19
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef LOGOSPRITE_HPP
#define LOGOSPRITE_HPP

#include "types.hpp"
#include "sprite/AnimatedSpriteBase.hpp"
#include "sprite/PositionedSpriteBase.hpp"

/**
 * @brief The developer's logo.
 */
class LogoSprite : public AnimatedSpriteBase, public PositionedSpriteBase {
private:
	// The animation has 3 phases: logo fades in, full brightness, fades out
	enum AnimationPhase {
		ANIM_FADEIN,
		ANIM_FULLBRIGHT,
		ANIM_FADEOUT,
	};
private:
	// The logo image to show
	static constexpr ImageId LOGO_IMAGE = IMG_PTERO;

	// Approx. 60 animation frames per second
	static constexpr std::clock_t TIMER_INTERVAL = 17;

	// Number of milliseconds it takes to fade in
	static constexpr double FADEIN_TIME = 1088.0;
	// Number of milliseconds the logo is shown at full brightness
	static constexpr double FULLBRIGHT_TIME = 612.0;
	// Number of milliseconds it takes to fade out
	static constexpr double FADEOUT_TIME = FADEIN_TIME;

	Timer m_timer;
	AnimationContinuous m_anim;
	AnimationPhase m_animPhase;

	uint8_t m_opacity;

	/**
	 * @brief Initializes an animation phase.
	 */
	void startAnimationPhase(AnimationPhase phase);
	/**
	 * @brief Based on the current animation phase goes to the next phase.
	 * 
	 * @remark If the current phase is the last one, stops the animation.
	 */
	void nextAnimationPhase();
	/**
	 * @brief Animation processing.
	 */
	void loopEventAnimation();
	/**
	 * @brief Fade in animation processing.
	 */
	void frameEventAnimFadein();
	/**
	 * @brief Full brightness "animation" processing.
	 */
	void frameEventAnimFullbright();
	/**
	 * @brief Fade out animation processing.
	 */
	void frameEventAnimFadeout();
protected:
	void repaintAsVisible(std::shared_ptr<ICanvas> canvas,
		const Rect& invalidRect) override;
public:
	LogoSprite(std::shared_ptr<IPaintingProxy> paintingProxy);

	/**
	 * @brief Returns the size (width and height) of the sprite.
	 * 
	 * @details This should always yield the same result as calling the Size2d
	 *          constructor as `Size2d(getW(), getH())`. However, calling this
	 *          method should be preferred, because this object may optimize it
	 *          to gain performance.
	 */
	Size2d getSize() override;
	/**
	 * @brief Starts a sprite animation.
	 * 
	 * @note If the method is called while an animation is running, the running
	 *       animation is stopped.
	 */
	void startAnimation() override;
	/**
	 * @brief Stops an animation if one is running.
	 * 
	 * @remarks No-op if animation is not running.
	 * 
	 *          It is NOT necessary to call this method after the animation
	 *          finishes. If it would be, the object itself should call it.
	 */
	void stopAnimation() override;
	/**
	 * @brief Event that happens every event loop iteration.
	 * 
	 * @details This method is mainly used to progress any running animations.
	 */
	void loopEvent() override;
};

#endif // LOGOSPRITE_HPP
