/**
 * @file StatusbarTextSprite.cpp
 * @author Tomáš Ludrovan
 * @brief StatusbarTextSprite class
 * @version 0.1
 * @date 2024-01-26
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "sprite/StatusbarTextSprite.hpp"

StatusbarTextSprite::StatusbarTextSprite(
	std::shared_ptr<IPaintingProxy> paintingProxy)
	: SpriteBase(paintingProxy)
	, m_text{""}
{}

Size2d StatusbarTextSprite::getSize()
{
	return paintingProxy->getTextSize(m_text, FONT);
}

void StatusbarTextSprite::repaint(std::shared_ptr<ICanvas> canvas,
	Rect& invalidRect)
{
	canvas->fillText(getX(), getY(), m_text, FONT);
}

const std::string & StatusbarTextSprite::getText() const
{
	return m_text;
}

void StatusbarTextSprite::setText(const std::string& value)
{
	invalidateBounds();
	m_text = value;
	invalidateBounds();
}
