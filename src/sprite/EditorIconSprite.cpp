/**
 * @file EditorIconSprite.cpp
 * @author Tomáš Ludrovan
 * @brief EditorIconSprite class
 * @version 0.1
 * @date 2024-01-26
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "sprite/EditorIconSprite.hpp"

EditorIconSprite::EditorIconSprite(
	std::shared_ptr<IPaintingProxy> paintingProxy, ImageId img)
	: SpriteBase(paintingProxy)
	, BoundedSpriteBase(paintingProxy)
	, PositionedSpriteBase(paintingProxy)
	, m_img{img}
	, m_costume{COSTUME_NORMAL}
{}

Size2d EditorIconSprite::getSize()
{
	return Size2d(FIXED_WIDTH, FIXED_HEIGHT);
}

void EditorIconSprite::repaint(std::shared_ptr<ICanvas> canvas,
	Rect& invalidRect)
{
	switch (m_costume) {
		case COSTUME_NORMAL:
			repaintCostumeNormal(canvas, invalidRect);
			break;
		case COSTUME_HOVER:
			repaintCostumeHover(canvas, invalidRect);
			break;
		case COSTUME_SELECTED:
			repaintCostumeSelected(canvas, invalidRect);
			break;
		case COSTUME_DISABLED:
			repaintCostumeDisabled(canvas, invalidRect);
			break;
	}

	invalidRect += getBounds();
}

void EditorIconSprite::paintImg(std::shared_ptr<ICanvas> canvas,
	Rect& invalidRect)
{
	Rect dstRect(getX() + IMG_MARGINS, getY() + IMG_MARGINS, FIXED_IMG_WIDTH,
		FIXED_IMG_HEIGHT);
	canvas->copyImage(m_img, dstRect);

	invalidRect += getBounds();
}

void EditorIconSprite::repaintCostumeNormal(std::shared_ptr<ICanvas> canvas,
	Rect& invalidRect)
{
	paintImg(canvas, invalidRect);
}

void EditorIconSprite::repaintCostumeHover(std::shared_ptr<ICanvas> canvas,
	Rect& invalidRect)
{
	Rect bounds = getBounds();

	canvas->setStrokingColor(Color::white());
	canvas->setStrokeWidth(1.0);
	canvas->strokeRectangle(bounds.x, bounds.y, bounds.w, bounds.h);
	paintImg(canvas, invalidRect);
}

void EditorIconSprite::repaintCostumeSelected(std::shared_ptr<ICanvas> canvas,
	Rect& invalidRect)
{
	Rect bounds = getBounds();

	canvas->setFillingColor(Color(0xff, 0xff, 0xff, 0x80));
	canvas->fillRectangle(bounds.x, bounds.y, bounds.w, bounds.h);
	paintImg(canvas, invalidRect);
}

void EditorIconSprite::repaintCostumeDisabled(std::shared_ptr<ICanvas> canvas,
	Rect& invalidRect)
{
	canvas->setColorMod(Color(0xff, 0xff, 0xff, 0x80));
	paintImg(canvas, invalidRect);
	canvas->setColorMod(Color(0xff, 0xff, 0xff, 0xff));
}

EditorIconSprite::Costume EditorIconSprite::getCostume() const
{
	return m_costume;
}

void EditorIconSprite::setCostume(Costume costume)
{
	invalidateBounds();
	m_costume = costume;
	invalidateBounds();
}
