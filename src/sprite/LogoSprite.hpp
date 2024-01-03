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

#include <string>

#include "types.hpp"
#include "sprite/SpriteBase.hpp"

class LogoSprite : public SpriteBase {
private:
	enum AnimationPhase {
		ANIM_FADEIN,
		ANIM_FULLBRIGHT,
		ANIM_FADEOUT,
	};
private:
	const std::string LOGO_TEXT = "Awesome Logo";
	const FontId LOGO_FONT = FONT_OLDENGL_72;

	const std::clock_t TIMER_INTERVAL = 17;

	const double FADEIN_TIME = 1088.0;
	const double FULLBRIGHT_TIME = 612.0;
	const double FADEOUT_TIME = FADEIN_TIME;

	Timer m_timer;
	AnimationContinuous m_anim;
	AnimationPhase m_animPhase;

	uint8_t m_opacity;

	void startAnimationPhase(AnimationPhase phase);
	void nextAnimationPhase();
	void loopEventAnimation();
	void loopEventAnimFadein();
	void loopEventAnimFullbright();
	void loopEventAnimFadeout();
public:
	LogoSprite();

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
	/**
	 * @brief Repaints an area of sprite.
	 * 
	 * @param canvas Canvas to paint onto.
	 * @param invalidRect Area to repaint.
	 */
	void repaint(std::shared_ptr<ICanvas> canvas, Rect& invalidRect) override;
};

#endif // LOGOSPRITE_HPP
