/**
 * @file BonusHpRecoverySprite.hpp
 * @author Tomáš Ludrovan
 * @brief BonusHpRecoverySprite class
 * @version 0.1
 * @date 2024-03-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef BONUSHPRECOVERYSPRITE_HPP
#define BONUSHPRECOVERYSPRITE_HPP

#include <string>

#include "types.hpp"
#include "sprite/BoundedSpriteBase.hpp"
#include "sprite/AnimatedSpriteBase.hpp"

class BonusHpRecoverySprite
	: public AnimatedSpriteBase
	, public BoundedSpriteBase
{
private:
	enum AnimationPhase {
		// Go up
		ANIM_GOUP,
		// Wait
		ANIM_STAYUP,
	};
private:
	static constexpr Color TEXT_COLOR = Color::white();
	static constexpr FontId TEXT_FONT = FONT_BRLNSTR_20;

	// Approx. 60 animation frames per second
	static constexpr std::clock_t TIMER_INTERVAL = 17;

	// Number of milliseconds it takes to go up
	static constexpr double GOUP_TIME = 1000.0;
	// Number of milliseconds the sprite waits after going up
	static constexpr double STAYUP_TIME = 800.0;

	// Number of pixels it goes up (negative, because up is towards 0)
	static constexpr double RISE_TOTAL = -40.0;

	Timer m_timer;
	AnimationContinuous m_anim;
	AnimationPhase m_animPhase;

	int m_hpRecovery;
	std::string m_hpRecoveryText; // Derived
	Rect m_bonusRect;

	double m_yIncrement;

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
	 * @brief Go up animation processing.
	 */
	void loopEventAnimGoup();
	/**
	 * @brief Stay up animation processing.
	 */
	void loopEventAnimStayup();
protected:
	void repaintAsVisible(std::shared_ptr<ICanvas> canvas,
		const Rect& invalidRect) override;
public:
	BonusHpRecoverySprite(std::shared_ptr<IPaintingProxy> paintingProxy);

	/**
	 * @brief Returns the rectangle that represents the bounds of the sprite.
	 */
	Rect getBounds() override;
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
	 * @brief Getter for the "HP recovery".
	 */
	int getHpRecovery() const;
	/**
	 * @brief Setter for the "HP recovery".
	 */
	void setHpRecovery(int value);

	/**
	 * @brief Getter for the "bonus rect".
	 * 
	 * @details Bonus rect is the rectangle where the bonus used to be.
	 */
	const Rect& getBonusRect() const;
	/**
	 * @brief Setter for the "bonus rect".
	 * 
	 * @copydetail BonusHpRecoverySprite::getBonusRect()
	 */
	void setBonusRect(const Rect& value);
};

#endif // BONUSHPRECOVERYSPRITE_HPP
