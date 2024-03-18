/**
 * @file BonusHpRecoverySprite.cpp
 * @author Tomáš Ludrovan
 * @brief BonusHpRecoverySprite class
 * @version 0.1
 * @date 2024-03-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "sprite/BonusHpRecoverySprite.hpp"

#include <sstream>

void BonusHpRecoverySprite::startAnimationPhase(AnimationPhase phase)
{
	// Animation phase length
	double animTime = 0.0;
	// Sprite brightness at the start of the animation phase
	double yIncrement = 0.0;

	// Set variable values
	switch (phase) {
		case ANIM_GOUP:
			animTime = GOUP_TIME;
			yIncrement = 0.0;
			break;
		case ANIM_STAYUP:
			animTime = STAYUP_TIME;
			yIncrement = RISE_TOTAL;
			break;
	}

	// Create continuous animation instance
	m_anim = AnimationContinuous(animTime);
	// Set the current animation phase
	m_animPhase = phase;
	// Set the initial Y increment
	m_yIncrement = yIncrement;
}

void BonusHpRecoverySprite::nextAnimationPhase()
{
	switch (m_animPhase) {
		case ANIM_GOUP:
			startAnimationPhase(ANIM_STAYUP);
			break;
		case ANIM_STAYUP:
			stopAnimation();
			break;
	}
}

void BonusHpRecoverySprite::loopEventAnimation()
{
	// Nothing to animate
	if (!isAnimationRunning()) return;

	if (m_timer.isLap()) {
		// Next animation frame

		if (m_anim.isCompleted()) {
			// Animation phase finished - start a new one (or stop animation)

			nextAnimationPhase();
			if (!isAnimationRunning()) return;
		}

		// Progress the animation phase
		switch (m_animPhase) {
			case ANIM_GOUP:
				loopEventAnimGoup();
				break;
			case ANIM_STAYUP:
				loopEventAnimStayup();
				break;
		}

		// Request repaint
		invalidate();
	}
}

void BonusHpRecoverySprite::loopEventAnimGoup()
{
	m_yIncrement = m_anim.getTime() * RISE_TOTAL;
}

void BonusHpRecoverySprite::loopEventAnimStayup()
{
	// Nothing to do
}

void BonusHpRecoverySprite::repaintAsVisible(std::shared_ptr<ICanvas> canvas,
	const Rect& invalidRect)
{
	(void)invalidRect;

	Rect bounds = getBounds();

	canvas->setFillingColor(TEXT_COLOR);
	canvas->fillText(bounds.x, bounds.y, m_hpRecoveryText, TEXT_FONT);
}

BonusHpRecoverySprite::BonusHpRecoverySprite(
	std::shared_ptr<IPaintingProxy> paintingProxy)
	: SpriteBase(paintingProxy)
	, AnimatedSpriteBase(paintingProxy)
	, BoundedSpriteBase(paintingProxy)
	, m_timer(TIMER_INTERVAL)
	, m_anim(0)
	, m_hpRecovery{0}
	, m_hpRecoveryText("")
	, m_bonusRect()
	, m_yIncrement{0}
{}

Rect BonusHpRecoverySprite::getBounds()
{
	Rect textRect(0, 0, paintingProxy->getTextSize(m_hpRecoveryText,
		TEXT_FONT));
	textRect.centerAt(m_bonusRect);
	textRect.y += static_cast<int>(m_yIncrement);
	return textRect;
}

void BonusHpRecoverySprite::startAnimation()
{
	AnimatedSpriteBase::startAnimation();
	startAnimationPhase(ANIM_GOUP);
	m_timer.reset();
}

void BonusHpRecoverySprite::stopAnimation()
{
	AnimatedSpriteBase::stopAnimation();
	m_yIncrement = RISE_TOTAL;
	invalidate();
}

void BonusHpRecoverySprite::loopEvent()
{
	loopEventAnimation();
}

int BonusHpRecoverySprite::getHpRecovery() const
{
	return propertyGetter(m_hpRecovery);
}

void BonusHpRecoverySprite::setHpRecovery(int value)
{
	propertySetterComparable(m_hpRecovery, value);

	std::stringstream ss;
	ss << "+ " << value << " HP";
	propertySetterComparable(m_hpRecoveryText, ss.str());
}

const Rect& BonusHpRecoverySprite::getBonusRect() const
{
	return propertyGetter(m_bonusRect);
}

void BonusHpRecoverySprite::setBonusRect(const Rect& value)
{
	propertySetterComparable(m_bonusRect, value);
}
