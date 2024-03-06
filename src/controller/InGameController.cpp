/**
 * @file InGameController.cpp
 * @author Tomáš Ludrovan
 * @brief InGameController class
 * @version 0.1
 * @date 2023-12-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "controller/InGameController.hpp"

#include <memory>

#include "core/Core.hpp"
#include "playerinput/PlayerInputFactory.hpp"

InGameController::InGameController(std::shared_ptr<ISysProxy> sysProxy,
	const std::shared_ptr<IStageSerializer> stage,
	const std::vector<std::shared_ptr<IPlayerInput>>& players)
	: GeneralControllerBase(sysProxy)
	, m_core{std::make_unique<Core>(stage, players)}
{}

void InGameController::createPlayerSprites()
{
	const auto& players = m_core->getPlayerStates();
	for (const auto& [id, state] : players) {
		auto playerSprite = std::make_unique<PlayerSprite>(sysProxy);
		playerSprite->setColor(Color::player(id));
		m_playerSprites[id] = std::move(playerSprite);
	}
}

void InGameController::createObstacleSprites()
{
	auto tm = m_viewport->getProjectionToScreenMatrix();

	for (const auto& obstacle : m_core->getObstaclesList()) {
		auto obstacleSprite = std::make_unique<ObstacleSprite>(sysProxy);

		PolygonF shape = obstacle.getTransformed(tm);

		obstacleSprite->setShape(shape);
		m_obstacleSprites.push_back(std::move(obstacleSprite));
	}
}

void InGameController::createStageBoundsSprite()
{
	Size2d screenSize = sysProxy->getPaintAreaSize();

	m_stageBoundsSprite = std::make_unique<HollowRectSprite>(sysProxy);
	m_stageBoundsSprite->setColor(Color::obstacle());
	m_stageBoundsSprite->setPos(0, 0);
	m_stageBoundsSprite->setBigRectWidth(screenSize.w);
	m_stageBoundsSprite->setBigRectHeight(screenSize.h);

	Rect stageRect(0, 0, m_core->getStageSize());
	RectF holeF = m_viewport->stageToScreen(static_cast<RectF>(stageRect));
	Rect hole = static_cast<Rect>(holeF);
	m_stageBoundsSprite->setHole(hole);
}

void InGameController::createPlayerStatusBarSprites()
{
	createStatusBarSprite();
	createPlayerHpBgSprites();
	createPlayerHpTextSprites();
}

void InGameController::createStatusBarSprite()
{
	Size2d screenSize = sysProxy->getPaintAreaSize();

	m_statusBarSprite = std::make_unique<OptionBarSprite>(sysProxy);
	m_statusBarSprite->setPos(screenSize.w - STATUS_BAR_WIDTH, 0);
	m_statusBarSprite->setFillingColor(Color::black());
	m_statusBarSprite->setStrokingColor(Color::white());
	m_statusBarSprite->setBorders(
		OptionBarSprite::Borders(true, false, false, false));
}

void InGameController::createPlayerHpBgSprites()
{
	const auto& players = m_core->getPlayerStates();

	for (auto& [id, state] : players) {
		auto spr = std::make_unique<OptionBarSprite>(sysProxy);
		Rect barRect = getPlayerHpBarRect(id);

		spr->setPos(barRect.getTopLeft());
		spr->setBarWidth(barRect.w);
		spr->setBarHeight(barRect.h);
		spr->setFillingColor(Color::player(id));
		spr->setBorders(
			OptionBarSprite::Borders(false, false, false, false));
		
		m_playerHpBgSprites[id] = std::move(spr);
	}
}

void InGameController::createPlayerHpTextSprites()
{
	const auto& players = m_core->getPlayerStates();

	for (auto& [id, state] : players) {
		auto spr = std::make_unique<TextSprite>(sysProxy);
		spr->setColor(Color::black());
		spr->setFont(PLAYER_HP_FONT);
		m_playerHpTextSprites[id] = std::move(spr);
	}
}

void InGameController::updateSprites()
{
	updatePlayerSprites();
	updatePlayerHpSprites();
}

void InGameController::updatePlayerSprites()
{
	auto plStates = m_core->getPlayerStates();

	for (const auto& [id, state] : plStates) {
		auto& spr = m_playerSprites[id];

		PointF posF = m_viewport->stageToScreen(PointF(state.x, state.y));
		double radiusF = state.size * m_viewport->getZoom();

		spr->setCenterPos(static_cast<Point>(posF));
		spr->setRadius(static_cast<int>(radiusF));
	}
}

void InGameController::updatePlayerHpSprites()
{
	auto plStates = m_core->getPlayerStates();

	for (const auto& [id, state] : plStates) {
		auto& spr = m_playerHpTextSprites[id];
		const auto& bgSpr = m_playerHpBgSprites.at(id);

		auto text = std::to_string(static_cast<int>(state.hp));

		Rect textRect(0, 0, sysProxy->getTextSize(text, PLAYER_HP_FONT));
		textRect.x = bgSpr->getBounds().getRight() - PLAYER_HP_RIGHT_MARGIN
			- textRect.w;
		textRect.y = bgSpr->getY() + PLAYER_HP_TOP_MARGIN;
		
		spr->setTextRect(textRect);
		spr->setText(text);
	}
}

void InGameController::initializeViewport()
{
	Rect stageArea = getStageAreaRect();
	Size2d stageSize = m_core->getStageSize();

	m_viewport = std::make_unique<StageViewport>(
		static_cast<Size2dF>(stageSize),
		static_cast<Size2dF>(stageArea.getSize()));

	Rect stageAreaMarg(
		stageArea.x + MIN_STAGE_LEFT_MARGIN,
		stageArea.y + MIN_STAGE_TOP_MARGIN,
		stageArea.w - (MIN_STAGE_LEFT_MARGIN + MIN_STAGE_RIGHT_MARGIN),
		stageArea.h - (MIN_STAGE_TOP_MARGIN + MIN_STAGE_BOTTOM_MARGIN)
	);

	if ((stageSize.w > stageAreaMarg.w) || (stageSize.h > stageAreaMarg.h)) {
		// The stage doesn't fit on the screen -- zoom out!

		// The ratios are between (0, 1)
		double wRatio = static_cast<double>(stageAreaMarg.w)
			/ static_cast<double>(stageSize.w);
		double hRatio = static_cast<double>(stageAreaMarg.h)
			/ static_cast<double>(stageSize.h);
		
		// Choose the smaller of the values
		m_viewport->setZoom(wRatio < hRatio ? wRatio : hRatio);
	}

	m_viewport->stageCenter();
}

Rect InGameController::getStageAreaRect()
{
	Rect res(0, 0, sysProxy->getPaintAreaSize());
	res.w -= STATUS_BAR_WIDTH;
	return res;
}

Rect InGameController::getPlayerHpBarRect(PlayerId playerId)
{
	Size2d screenSize = sysProxy->getPaintAreaSize();
	int barHeight = PLAYER_HP_TOP_MARGIN + getPlayerHpTextHeight()
		+ PLAYER_HP_BOTTOM_MARGIN;

	Rect res(
		screenSize.w - STATUS_BAR_WIDTH,
		barHeight * playerId,
		STATUS_BAR_WIDTH,
		barHeight
	);
	return res;
}

int InGameController::getPlayerHpTextHeight()
{
	return sysProxy->getTextSize("Lorem ipsum", PLAYER_HP_FONT).h;
}

void InGameController::onStarted()
{
	GeneralControllerBase::onStarted();

	// This will initialize the core
	// FIXME: remove?
	m_core->loopEvent();

	initializeViewport();
	createPlayerSprites();
	createObstacleSprites();
	createStageBoundsSprite();
	createPlayerStatusBarSprites();
	updateSprites();
}

void InGameController::onLoop()
{
	m_core->loopEvent();
	updateSprites();
}

void InGameController::onPaint(std::shared_ptr<ICanvas> canvas,
	const Rect& invalidRect)
{
	for (auto& [id, spr] : m_playerSprites) {
		spr->repaint(canvas, invalidRect);
	}

	for (auto& spr : m_obstacleSprites) {
		spr->repaint(canvas, invalidRect);
	}

	m_stageBoundsSprite->repaint(canvas, invalidRect);

	m_statusBarSprite->repaint(canvas, invalidRect);

	for (auto& [id, spr] : m_playerHpBgSprites) {
		spr->repaint(canvas, invalidRect);
	}

	for (auto& [id, spr] : m_playerHpTextSprites) {
		spr->repaint(canvas, invalidRect);
	}
}
