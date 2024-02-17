/**
 * @file StageItemSprite.cpp
 * @author Tomáš Ludrovan
 * @brief StageItemSprite class
 * @version 0.1
 * @date 2024-02-17
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "sprite/StageItemSprite.hpp"

void StageItemSprite::repaintColor(std::shared_ptr<ICanvas> canvas,
	const Rect& invalidRect, const Color& borderColor)
{
	(void)invalidRect;

	int textX, textY;

	// Border
	canvas->setStrokeWidth(1.0);
	canvas->setStrokingColor(borderColor);
	canvas->strokeRectangle(getX(), getY(), m_borderWidth, getH());

	// Stage title
	textX = getX() + PADDING_LEFT;
	textY = getY() + PADDING_VERT;
	canvas->setFillingColor(STAGE_TITLE_TEXT_COLOR);
	canvas->fillText(textX, textY, m_stageTitle, STAGE_TITLE_FONT);

	// Stage path
	int stageTitleH = paintingProxy->getTextSize(m_stageTitle,
		STAGE_TITLE_FONT).h;
	// textX does not change
	textY += stageTitleH + TEXT_SPACING;
	canvas->setFillingColor(STAGE_PATH_TEXT_COLOR);
	canvas->fillText(textX, textY, m_stagePath, STAGE_PATH_FONT);
}

void StageItemSprite::repaintNormal(std::shared_ptr<ICanvas> canvas,
	const Rect& invalidRect)
{
	repaintColor(canvas, invalidRect, BORDER_COLOR_NORMAL);
}

void StageItemSprite::repaintHover(std::shared_ptr<ICanvas> canvas,
	const Rect& invalidRect)
{
	repaintColor(canvas, invalidRect, BORDER_COLOR_HOVER);
}

void StageItemSprite::repaintAsVisible(std::shared_ptr<ICanvas> canvas,
	const Rect& invalidRect)
{
	switch (m_costume) {
		case COSTUME_NORMAL:
			repaintNormal(canvas, invalidRect);
			break;
		case COSTUME_HOVER:
			repaintHover(canvas, invalidRect);
			break;
	}
}

StageItemSprite::StageItemSprite(std::shared_ptr<IPaintingProxy> paintingProxy)
	: SpriteBase(paintingProxy)
	, BoundedSpriteBase(paintingProxy)
	, PositionedSpriteBase(paintingProxy)
{}

Size2d StageItemSprite::getSize()
{
	Size2d stageTitleSize = paintingProxy->getTextSize(m_stageTitle,
		STAGE_TITLE_FONT);
	Size2d stagePathSize = paintingProxy->getTextSize(m_stagePath,
		STAGE_PATH_FONT);

	int w = std::max(std::max(stageTitleSize.w, stagePathSize.w),
		m_borderWidth);
	int h = PADDING_VERT + stageTitleSize.h + TEXT_SPACING + stagePathSize.h
		+ PADDING_VERT;
	
	return Size2d(w, h);
}

StageItemSprite::Costume StageItemSprite::getCostume() const
{
	return propertyGetter(m_costume);
}

void StageItemSprite::setCostume(StageItemSprite::Costume value)
{
	propertySetterComparable(m_costume, value);
}

const std::string& StageItemSprite::getStageTitle() const
{
	return propertyGetter(m_stageTitle);
}

void StageItemSprite::setStageTitle(const std::string& value)
{
	propertySetterComparable(m_stageTitle, value);
}

const std::string& StageItemSprite::getStagePath() const
{
	return propertyGetter(m_stagePath);
}

void StageItemSprite::setStagePath(const std::string& value)
{
	propertySetterComparable(m_stagePath, value);
}

int StageItemSprite::getBorderWidth() const
{
	return propertyGetter(m_borderWidth);
}

void StageItemSprite::setBorderWidth(int value)
{
	propertySetterComparable(m_borderWidth, value);
}
