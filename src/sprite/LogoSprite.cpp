/**
 * @file LogoSprite.cpp
 * @author Tomáš Ludrovan
 * @brief LogoSprite class
 * @version 0.1
 * @date 2023-12-19
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "sprite/LogoSprite.hpp"

LogoSprite::LogoSprite(std::shared_ptr<IPaintingProxy> paintingProxy)
	: AnimatedSpriteBase(paintingProxy)
	, m_timer(TIMER_INTERVAL)
	, m_anim(0)
{}

void LogoSprite::startAnimationPhase(AnimationPhase phase)
{
	// Animation phase length
	double animTime = 0.0;
	// Sprite brightness at the start of the animation phase
	uint8_t opacity = 0;

	// Set variable values
	switch (phase) {
		case ANIM_FADEIN:
			animTime = FADEIN_TIME;
			opacity = 0;
			break;
		case ANIM_FULLBRIGHT:
			animTime = FULLBRIGHT_TIME;
			opacity = 0xff;
			break;
		case ANIM_FADEOUT:
			animTime = FADEOUT_TIME;
			opacity = 0xff;
			break;
	}

	// Create continuous animation instance
	m_anim = AnimationContinuous(animTime);
	// Set the current animation phase
	m_animPhase = phase;
	// Set the initial opacity (brightness)
	m_opacity = opacity;
}

void LogoSprite::nextAnimationPhase()
{
	switch (m_animPhase) {
		case ANIM_FADEIN:
			startAnimationPhase(ANIM_FULLBRIGHT);
			break;
		case ANIM_FULLBRIGHT:
			startAnimationPhase(ANIM_FADEOUT);
			break;
		case ANIM_FADEOUT:
			stopAnimation();
			break;
	}
}

void LogoSprite::loopEventAnimation()
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
			case ANIM_FADEIN:
				frameEventAnimFadein();
				break;
			case ANIM_FULLBRIGHT:
				frameEventAnimFullbright();
				break;
			case ANIM_FADEOUT:
				frameEventAnimFadeout();
				break;
		}

		// Request repaint
		invalidateBounds();
	}
}

void LogoSprite::frameEventAnimFadein()
{
	m_opacity = m_anim.getTime() * 0xff;
}

void LogoSprite::frameEventAnimFullbright()
{
	m_opacity = 0xff;
}

void LogoSprite::frameEventAnimFadeout()
{
	m_opacity = (1.0 - m_anim.getTime()) * 0xff;
}

Size2d LogoSprite::getSize()
{
	return paintingProxy->getTextSize(LOGO_TEXT, LOGO_FONT);
}

void LogoSprite::startAnimation()
{
	AnimatedSpriteBase::startAnimation();
	startAnimationPhase(ANIM_FADEIN);
	m_timer.reset();
}

void LogoSprite::stopAnimation()
{
	AnimatedSpriteBase::stopAnimation();
	m_opacity = 0;
	invalidateBounds();
}

void LogoSprite::loopEvent()
{
	loopEventAnimation();
}

void LogoSprite::repaint(std::shared_ptr<ICanvas> canvas, Rect& invalidRect)
{
	// Paint
	canvas->setFillingColor(Color(m_opacity, m_opacity, m_opacity));
	canvas->fillText(getX(), getY(), LOGO_TEXT, LOGO_FONT);
	// We might have repainted an area bigger than invalidRect
	invalidRect += getBounds();
}
