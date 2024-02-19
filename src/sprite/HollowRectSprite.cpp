/**
 * @file HollowRectSprite.cpp
 * @author Tomáš Ludrovan
 * @brief HollowRectSprite class
 * @version 0.1
 * @date 2024-02-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "sprite/HollowRectSprite.hpp"

void HollowRectSprite::repaintAsVisible(std::shared_ptr<ICanvas> canvas,
	const Rect& invalidRect)
{
	(void)invalidRect;

	Rect bounds = getBounds();

	canvas->setFillingColor(m_color);

	if (bounds.x < m_hole.x) {
		// Left rectangle

		Rect r(Point(bounds.x, bounds.y), Point(m_hole.x, bounds.getBottom()));
		canvas->fillRectangle(r.x, r.y, r.w, r.h);
	}
	if (bounds.y < m_hole.y) {
		// Top rectangle

		Rect r(Point(bounds.x, bounds.y), Point(bounds.getRight(), m_hole.y));
		canvas->fillRectangle(r.x, r.y, r.w, r.h);
	}
	if (bounds.getRight() > m_hole.getRight()) {
		// Right rectangle

		Rect r(Point(m_hole.getRight(), bounds.y), Point(bounds.getRight(), bounds.getBottom()));
		canvas->fillRectangle(r.x, r.y, r.w, r.h);
	}
	if (bounds.getBottom() > m_hole.getBottom()) {
		// Bottom rectangle

		Rect r(Point(bounds.x, m_hole.getBottom()), Point(bounds.getRight(), bounds.getBottom()));
		canvas->fillRectangle(r.x, r.y, r.w, r.h);
	}
}

HollowRectSprite::HollowRectSprite(std::shared_ptr<IPaintingProxy> paintingProxy)
	: SpriteBase(paintingProxy)
	, BoundedSpriteBase(paintingProxy)
	, PositionedSpriteBase(paintingProxy)
	, m_bigRectWidth{0}
	, m_bigRectHeight{0}
{}

Size2d HollowRectSprite::getSize()
{
	return Size2d(m_bigRectWidth, m_bigRectHeight);
}

const Color& HollowRectSprite::getColor() const
{
	return propertyGetter(m_color);
}

void HollowRectSprite::setColor(const Color& value)
{
	propertySetterComparable(m_color, value);
}

int HollowRectSprite::getBigRectWidth() const
{
	return propertyGetter(m_bigRectWidth);
}

void HollowRectSprite::setBigRectWidth(int value)
{
	propertySetterComparable(m_bigRectWidth, value);
}

int HollowRectSprite::getBigRectHeight() const
{
	return propertyGetter(m_bigRectHeight);
}

void HollowRectSprite::setBigRectHeight(int value)
{
	propertySetterComparable(m_bigRectHeight, value);
}

const Rect& HollowRectSprite::getHole() const
{
	return propertyGetter(m_hole);
}

void HollowRectSprite::setHole(const Rect& value)
{
	propertySetterComparable(m_hole, value);
}
