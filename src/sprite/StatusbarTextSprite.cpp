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
	, BoundedSpriteBase(paintingProxy)
	, PositionedSpriteBase(paintingProxy)
	, m_text{""}
{}

Size2d StatusbarTextSprite::getSize()
{
	return paintingProxy->getTextSize(m_text, FONT);
}

void StatusbarTextSprite::repaintAsVisible(std::shared_ptr<ICanvas> canvas,
	const Rect& invalidRect)
{
	(void)invalidRect;

	canvas->setFillingColor(Color::white());
	canvas->fillText(getX(), getY(), m_text, FONT);
}

const std::string & StatusbarTextSprite::getText() const
{
	return m_text;
}

void StatusbarTextSprite::setText(const std::string& value)
{
	invalidate();
	m_text = value;
	invalidate();
}
