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
	: SpriteBase(paintingProxy)
	, m_timer(TIMER_INTERVAL)
	, m_anim(0)
{}

void LogoSprite::startAnimationPhase(AnimationPhase phase)
{
	double animTime = 0.0;
	uint8_t opacity = 0;

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

	m_anim = AnimationContinuous(animTime);
	m_animPhase = phase;
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
	if (!isAnimationRunning()) return;

	if (m_timer.isLap()) {
		if (m_anim.isCompleted()) {
			nextAnimationPhase();
		}
		switch (m_animPhase) {
			case ANIM_FADEIN:
				loopEventAnimFadein();
				break;
			case ANIM_FULLBRIGHT:
				loopEventAnimFullbright();
				break;
			case ANIM_FADEOUT:
				loopEventAnimFadeout();
				break;
		}
		invalidateBounds();
	}
}

void LogoSprite::loopEventAnimFadein()
{
	m_opacity = m_anim.getTime() * 0xff;
}

void LogoSprite::loopEventAnimFullbright()
{
	m_opacity = 0xff;
}

void LogoSprite::loopEventAnimFadeout()
{
	m_opacity = (1.0 - m_anim.getTime()) * 0xff;
}

Size2d LogoSprite::getSize()
{
	return paintingProxy->getTextSize(LOGO_TEXT, LOGO_FONT);
}

void LogoSprite::startAnimation()
{
	SpriteBase::startAnimation();
	startAnimationPhase(ANIM_FADEIN);
	m_timer.reset();
}

void LogoSprite::stopAnimation()
{
	SpriteBase::stopAnimation();
	m_opacity = 0;
	invalidateBounds();
}

void LogoSprite::loopEvent()
{
	loopEventAnimation();
}

void LogoSprite::repaint(std::shared_ptr<ICanvas> canvas, Rect& invalidRect)
{
	canvas->setFillingColor(Color(m_opacity, m_opacity, m_opacity));
	canvas->fillText(getX(), getY(), LOGO_TEXT, LOGO_FONT);
	invalidRect += getBounds();
}
