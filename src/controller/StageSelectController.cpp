/**
 * @file StageSelectController.cpp
 * @author Tomáš Ludrovan
 * @brief StageSelectController class
 * @version 0.1
 * @date 2024-02-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "controller/StageSelectController.hpp"

#include "stageserializer/StageSerializerFactory.hpp"

void StageSelectController::initializeSprites()
{
	createSprites();
	positionSprites();
}

void StageSelectController::createSprites()
{
	m_cancelButtonSprite = std::make_unique<TextButtonSprite>(sysProxy);

	m_cancelButtonSprite->setText(CANCEL_BUTTON_TEXT);
}

void StageSelectController::positionSprites()
{
	// Cancel button
	m_cancelButtonSprite->setPos(getCancelButtonRect().getTopLeft());
}

void StageSelectController::cancelButtonClick()
{
	m_isSelectedStageValid = false;
	finishedEvent();
}

void StageSelectController::stageItemClick(int itemIdx)
{
	m_selectedStage = m_stageIds[itemIdx];
	m_isSelectedStageValid = true;
	finishedEvent();
}

void StageSelectController::updateAllSprites()
{
	checkCancelButtonMouseHover(sysProxy->getMousePos());
	checkStageItemMouseHover(sysProxy->getMousePos());
}

void StageSelectController::updateStagesList()
{
	m_stageIds.clear();
	m_stageItemSprites.clear();

	auto serializer = StageSerializerFactory::createDefault();
	auto titles = serializer->getAllTitles();

	for (const auto& titlePair : titles) {
		const auto& stageId = titlePair.first;
		const auto& stageTitle = titlePair.second;
		
		m_stageIds.push_back(stageId);
		addStageItemSprite(stageTitle,
			serializer->getStageCanonicalId(stageId));
	}
}

void StageSelectController::addStageItemSprite(const std::string& title,
	const std::string& path)
{
	auto spr = std::make_unique<StageItemSprite>(sysProxy);
	spr->setStageTitle(title);
	spr->setStagePath(path);
	spr->setBorderWidth(getStageItemWidth());
	spr->setPos(getStageItemRect(m_stageItemSprites.size()).getTopLeft());
	m_stageItemSprites.push_back(std::move(spr));
}

Rect StageSelectController::getStageItemRect(int itemIdx)
{
	int paintAreaW = sysProxy->getPaintAreaSize().w;

	int stageItemW = getStageItemWidth();
	int stageItemH = StageItemSprite::BORDER_HEIGHT;
	// Centered
	int stageItemX = (paintAreaW - stageItemW) / 2;
	int stageItemY = STAGE_ITEM_TOP_MARGIN + (itemIdx * STAGE_ITEM_DISTANCES);

	Rect res(stageItemX, stageItemY, stageItemW, stageItemH);
	return res;
}

Rect StageSelectController::getCancelButtonRect()
{
	Size2d screenSize = sysProxy->getPaintAreaSize();

	// Horizontally centered
	int x = (screenSize.w - TextButtonSprite::BUTTON_WIDTH) / 2;
	int y = screenSize.h - CANCEL_BUTTON_BOTTOM_MARGIN
		- TextButtonSprite::BUTTON_HEIGHT;
	Rect res(x, y, TextButtonSprite::BUTTON_WIDTH,
		TextButtonSprite::BUTTON_HEIGHT);
	return res;
}

void StageSelectController::checkCancelButtonMouseHover(const Point& mouse)
{
	if (getCancelButtonRect().containsPoint(mouse)) {
		m_cancelButtonSprite->setCostume(TextButtonSprite::COSTUME_HOVER);
	} else {
		m_cancelButtonSprite->setCostume(TextButtonSprite::COSTUME_NORMAL);
	}
}

void StageSelectController::checkStageItemMouseHover(const Point& mouse)
{
	for (size_t i = 0; i < m_stageIds.size(); i++) {
		if (getStageItemRect(i).containsPoint(mouse)) {
			m_stageItemSprites[i]->setCostume(StageItemSprite::COSTUME_HOVER);
		} else {
			m_stageItemSprites[i]->setCostume(StageItemSprite::COSTUME_NORMAL);
		}
	}
}

void StageSelectController::checkCancelButtonClick(const Point& mouse)
{
	if (getCancelButtonRect().containsPoint(mouse)) {
		cancelButtonClick();
	}
}

void StageSelectController::checkStageItemClick(const Point& mouse)
{
	for (size_t i = 0; i < m_stageIds.size(); i++) {
		if (getStageItemRect(i).containsPoint(mouse)) {
			stageItemClick(i);
			break;
		}
	}
}

int StageSelectController::getStageItemWidth()
{
	return static_cast<int>(sysProxy->getPaintAreaSize().w
		* STAGE_ITEM_WIDTH_REL);
}

void StageSelectController::onStarted()
{
	initializeSprites();
	updateStagesList();
	updateAllSprites();
}

void StageSelectController::onMouseMove(int x, int y)
{
	checkCancelButtonMouseHover(Point(x, y));
	checkStageItemMouseHover(Point(x, y));
}

void StageSelectController::onMouseBtnDown(MouseBtn btn, int x, int y)
{
	if (btn == BTN_LEFT) {
		checkCancelButtonClick(Point(x, y));
		checkStageItemClick(Point(x, y));
	}
}

void StageSelectController::onPaint(std::shared_ptr<ICanvas> canvas,
	const Rect& invalidRect)
{
	m_cancelButtonSprite->repaint(canvas, invalidRect);

	for (auto& spr : m_stageItemSprites) {
		spr->repaint(canvas, invalidRect);
	}
}

StageSelectController::StageSelectController(
	std::shared_ptr<ISysProxy> sysProxy,
	IStageSerializer::IdType& selectedStage, bool& isSelectedStageValid)
	: GeneralControllerBase(sysProxy)
	, m_selectedStage{selectedStage}
	, m_isSelectedStageValid{isSelectedStageValid}
{}
