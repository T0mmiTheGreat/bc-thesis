/**
 * @file MainMenuItemSprite.cpp
 * @author Tomáš Ludrovan
 * @brief MainMenuItemSprite class
 * @version 0.1
 * @date 2023-12-19
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "sprite/MainMenuItemSprite.hpp"

#include <cassert>

MainMenuItemSprite::MainMenuItemSprite()
	: SpriteBase()
	, m_timer(17)
	, m_scale{1.0}
{}

Size2d MainMenuItemSprite::getSize()
{
	return Size2d(RECT_WIDTH_MAX, RECT_HEIGHT_MAX);
}

void MainMenuItemSprite::repaint(std::shared_ptr<ICanvas> canvas, Rect& invalidRect)
{
	// Bounds
	canvas->setFillingColor(Color(0, 0, 0, 0));
	canvas->setStrokingColor(Color(0xff, 0xff, 0xff, 0xff));
	int marginHorz = (RECT_WIDTH_MAX - (RECT_WIDTH_NORMAL * m_scale));
	int marginVert = (RECT_HEIGHT_MAX - (RECT_HEIGHT_NORMAL * m_scale));
	int rectX = x + marginHorz;
	int rectY = y + marginVert;
	int rectW = RECT_WIDTH_MAX - 2*marginHorz;
	int rectH = RECT_HEIGHT_MAX - 2*marginVert;
	canvas->drawRectangle(rectX, rectY, rectW, rectH);

	// Text
	canvas->setFillingColor(Color(0xff, 0xff, 0xff, 0xff));
	Size2d textSize = sysProxy->getTextSize(m_text, FONT);
	int textX = x + ((RECT_WIDTH_MAX - textSize.w) / 2);
	int textY = y + ((RECT_HEIGHT_MAX - textSize.h) / 2);
	canvas->fillText(textX, textY, m_text, FONT);

	invalidRect += getBounds();
}

void MainMenuItemSprite::startAnimation()
{
	SpriteBase::startAnimation();
	m_timer.reset();
	m_frameNumber = 0;
	switch (m_animationType) {
		case ANIM_GROW:
			m_scale = 1.0;
			break;
		case ANIM_SHRINK:
			m_scale = MAX_SCALE;
			break;
	}
}

void MainMenuItemSprite::stopAnimation()
{
	SpriteBase::stopAnimation();
	switch (m_animationType) {
		case ANIM_GROW:
			m_scale = MAX_SCALE;
			break;
		case ANIM_SHRINK:
			m_scale = 1.0;
			break;
	}
}

void MainMenuItemSprite::frameEvent()
{
	if (!isAnimationRunning()) return;

	if (m_timer.isLap()) {
		m_frameNumber++;

		double animProgress;

		switch (m_animationType) {
			case ANIM_GROW:
				if (m_frameNumber >= GROWTH_FRAME_COUNT) {
					stopAnimation();
					return;
				}
				animProgress = static_cast<double>(m_frameNumber) / static_cast<double>(GROWTH_FRAME_COUNT - 1);
				m_scale = 1.0 + ((animProgress) * (MAX_SCALE - 1.0));
				break;
			case ANIM_SHRINK:
				if (m_frameNumber >= SHRINK_FRAME_COUNT) {
					stopAnimation();
					return;
				}
				animProgress = static_cast<double>(m_frameNumber) / static_cast<double>(SHRINK_FRAME_COUNT - 1);
				m_scale = 1.0 + ((1.0 - animProgress) * (MAX_SCALE - 1.0));
				break;
		}
	}
	sysProxy->invalidateRect(getBounds());
}

const std::string & MainMenuItemSprite::getText() const
{
	return m_text;
}

void MainMenuItemSprite::setText(const std::string& value)
{
	m_text = value;
	sysProxy->invalidateRect(getBounds());
}

MainMenuItemSprite::AnimationType MainMenuItemSprite::getAnimationType() const
{
	return m_animationType;
}

void MainMenuItemSprite::setAnimationType(MainMenuItemSprite::AnimationType value)
{
	assert(!isAnimationRunning());

	m_animationType = value;
}

void MainMenuItemSprite::mouseMoveEvent(int x, int y)
{
	Rect spriteBounds = getBounds();
	if ((spriteBounds.x <= x && x <= spriteBounds.getRight()) && (spriteBounds.y <= y && y <= spriteBounds.getBottom())) {
		if (isAnimationRunning() && getAnimationType() == ANIM_SHRINK) {
			stopAnimation();
		}
		if (!isAnimationRunning() && m_scale < MAX_SCALE) {
			setAnimationType(ANIM_GROW);
			startAnimation();
		}
	}
	else {
		if (isAnimationRunning() && getAnimationType() == ANIM_GROW) {
			stopAnimation();
		}
		if (!isAnimationRunning() && m_scale > 1.0) {
			setAnimationType(ANIM_SHRINK);
			startAnimation();
		}
	}
}
