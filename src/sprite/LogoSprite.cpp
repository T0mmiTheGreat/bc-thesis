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

LogoSprite::LogoSprite():
	SpriteBase(),
	m_timer(17)
{}

int LogoSprite::getW()
{
	return 480;
}

int LogoSprite::getH()
{
	return 360;
}

void LogoSprite::startAnimation()
{
	SpriteBase::startAnimation();
	m_timer.reset();
	m_opacity = 0;
	m_frameNumber = 0;
}

void LogoSprite::stopAnimation()
{
	SpriteBase::stopAnimation();
	m_opacity = 0;
	invalidateBounds();
}

void LogoSprite::frameEvent()
{
	if (!isAnimationRunning()) return;

	if (m_timer.isLap()) {
		m_frameNumber++;
		if (m_frameNumber <= 0x3f) {
			m_opacity = m_frameNumber * 4;
		}
		else if (m_frameNumber <= 100) {
			m_opacity = 0xff;
		}
		else if (m_frameNumber <= 100 + 0x3f) {
			m_opacity = 0xff - ((m_frameNumber - 100) * 4);
		}
		else {
			m_opacity = 0;
			stopAnimation();
		}
		invalidateBounds();
	}
}

void LogoSprite::repaint(std::shared_ptr<ICanvas> canvas, Rect& invalidRect)
{
	canvas->setFillingColor(Color(m_opacity, m_opacity, m_opacity, 0xff));
	canvas->fillCircle(x + 195, y + 100, 25);
	canvas->fillCircle(x + 480 - 195, y + 100, 25);
	invalidRect += getBounds();
}
