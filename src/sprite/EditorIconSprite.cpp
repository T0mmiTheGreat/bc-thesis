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
	: ImageSprite(paintingProxy, img)
	, m_costume{COSTUME_NORMAL}
{}

EditorIconSprite::EditorIconSprite(
	std::shared_ptr<IPaintingProxy> paintingProxy, ImageId img,
	const Rect& imgBounds)
	: ImageSprite(paintingProxy, img, imgBounds)
	, m_costume{COSTUME_NORMAL}
{}

Size2d EditorIconSprite::getSize()
{
	return ImageSprite::getSize();
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
}

void EditorIconSprite::repaintCostumeNormal(std::shared_ptr<ICanvas> canvas,
	Rect& invalidRect)
{
	ImageSprite::repaint(canvas, invalidRect);
}

void EditorIconSprite::repaintCostumeHover(std::shared_ptr<ICanvas> canvas,
	Rect& invalidRect)
{
	Rect bounds = getBounds();

	canvas->setStrokingColor(Color::white());
	canvas->setStrokeWidth(1.0);
	canvas->strokeRectangle(bounds.x, bounds.y, bounds.w, bounds.h);
	ImageSprite::repaint(canvas, invalidRect);
}

void EditorIconSprite::repaintCostumeSelected(std::shared_ptr<ICanvas> canvas,
	Rect& invalidRect)
{
	Rect bounds = getBounds();

	canvas->setFillingColor(Color(0xff, 0xff, 0xff, 0x80));
	canvas->fillRectangle(bounds.x, bounds.y, bounds.w, bounds.h);
	ImageSprite::repaint(canvas, invalidRect);
}

void EditorIconSprite::repaintCostumeDisabled(std::shared_ptr<ICanvas> canvas,
	Rect& invalidRect)
{
	canvas->setColorMod(Color(0xff, 0xff, 0xff, 0x80));
	ImageSprite::repaint(canvas, invalidRect);
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
