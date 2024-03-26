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

#include "controller/ControllerFactory.hpp"
#include "core/Core.hpp"
#include "playerinput/PlayerInputFactory.hpp"

InGameController::InGameController(std::shared_ptr<ISysProxy> sysProxy,
	const GameSetupData& gsdata)
	: GeneralControllerBase(sysProxy)
	, m_core{std::make_unique<Core>(gsdata)}
{}

void InGameController::createPlayerSprite(PlayerId id)
{
	auto playerSprite = std::make_unique<PlayerSprite>(sysProxy);
	playerSprite->setColor(Color::player(id));
	m_playerSprites[id] = std::move(playerSprite);
}

void InGameController::createBonusSprite(BonusId id, const PointF& pos)
{
	auto bonusSprite = std::make_unique<BonusSprite>(sysProxy);
	double radiusF = BONUS_RADIUS * m_viewport->getZoom();
	PointF posTf = m_viewport->stageToScreen(pos);
	bonusSprite->setRadius(static_cast<int>(radiusF));
	bonusSprite->setCenterPos(static_cast<Point>(posTf));
	m_bonusSprites[id] = std::move(bonusSprite);
}

void InGameController::createPlayerHpBgSprite(PlayerId id)
{
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

void InGameController::createPlayerHpTextSprite(PlayerId id)
{
	auto spr = std::make_unique<TextSprite>(sysProxy);
	spr->setColor(Color::black());
	spr->setFont(PLAYER_HP_FONT);
	m_playerHpTextSprites[id] = std::move(spr);
}

void InGameController::createObstacleSprite(const PolygonF& shape)
{
	auto tm = m_viewport->getProjectionToScreenMatrix();
	PolygonF shapeTf = shape.getTransformed(tm);

	auto obstacleSprite = std::make_unique<ObstacleSprite>(sysProxy);
	obstacleSprite->setShape(shapeTf);
	m_obstacleSprites.push_back(std::move(obstacleSprite));
}

void InGameController::createSprites()
{
	createStageBoundsSprite();
	createStatusBarSprite();
	createExitBtnSprite();
	createGameOverTextSprite();
}

void InGameController::createStageBoundsSprite()
{
	Size2d screenSize = sysProxy->getPaintAreaSize();

	m_stageBoundsSprite = std::make_unique<HollowRectSprite>(sysProxy);
	m_stageBoundsSprite->setColor(Color::obstacle());
	m_stageBoundsSprite->setPos(0, 0);
	m_stageBoundsSprite->setBigRectWidth(screenSize.w);
	m_stageBoundsSprite->setBigRectHeight(screenSize.h);
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

void InGameController::createExitBtnSprite()
{
	Size2d screenSize = sysProxy->getPaintAreaSize();

	m_exitBtnSprite = std::make_unique<OptionButtonSprite>(sysProxy);
	m_exitBtnSprite->setSize(OptionButtonSprite::BUTTON_SIZE_NARROWED);

	// Centered horizontally at the status bar
	int spriteW = OptionButtonSprite::BUTTON_SIZE_NARROWED.w;
	int spriteH = OptionButtonSprite::BUTTON_SIZE_NARROWED.h;
	int spriteRight = (STATUS_BAR_WIDTH - spriteW) / 2;
	int x = screenSize.w - spriteRight - spriteW;
	int y = screenSize.h - EXIT_BTN_BOTTOM_MARGIN - spriteH;
	m_exitBtnSprite->setPos(x, y);

	m_exitBtnSprite->setText(EXIT_BTN_TEXT);

	checkExitBtnMouseHover(sysProxy->getMousePos());
}

void InGameController::createGameOverTextSprite()
{
	Rect stageAreaRect = getStageAreaRect();

	m_gameOverTextSprite = std::make_unique<TextSprite>(sysProxy);
	m_gameOverTextSprite->setTextRect(stageAreaRect);
	m_gameOverTextSprite->setColor(GAME_OVER_TEXT_COLOR);
	m_gameOverTextSprite->setFont(GAME_OVER_TEXT_FONT);
}

void InGameController::checkExitBtnMouseHover(const Point& mouse)
{
	if (m_exitBtnSprite->getBounds().containsPoint(mouse)) {
		m_exitBtnSprite->setCostume(OptionButtonSprite::COSTUME_HOVER);
	} else {
		m_exitBtnSprite->setCostume(OptionButtonSprite::COSTUME_NORMAL);
	}
}

void InGameController::checkExitBtnClick(const Point& mouse)
{
	if (m_exitBtnSprite->getBounds().containsPoint(mouse)) {
		exitBtnClick();
	}
}

void InGameController::exitBtnClick()
{
	finishedEvent();
}

void InGameController::updatePlayerPos(PlayerId id, const PointF& pos)
{
	PointF posTf = m_viewport->stageToScreen(pos);
	m_playerSprites[id]->setCenterPos(static_cast<Point>(posTf));
}

void InGameController::updatePlayerHp(PlayerId id, double hp)
{
	auto& spr = m_playerHpTextSprites[id];
	const auto& bgSpr = m_playerHpBgSprites.at(id);

	auto text = std::to_string(static_cast<int>(hp * PLAYER_HP_FACTOR));

	Rect textRect(0, 0, sysProxy->getTextSize(text, PLAYER_HP_FONT));
	textRect.x = bgSpr->getBounds().getRight() - PLAYER_HP_RIGHT_MARGIN
		- textRect.w;
	textRect.y = bgSpr->getY() + PLAYER_HP_TOP_MARGIN;
	
	spr->setTextRect(textRect);
	spr->setText(text);
}

void InGameController::updatePlayerSize(PlayerId id, double size)
{
	// Size transformed
	double sizeTf = size * m_viewport->getZoom();
	// Truncate the transformed size to get the new radius value
	int newRadius = static_cast<int>(sizeTf);
	
	auto& spr = m_playerSprites[id];
	Point sprPos(spr->getX(), spr->getY());
	int sprRadius = spr->getRadius();

	// The sprite "growth"
	int sprRadiusDelta = newRadius - sprRadius;

	// Adjust sprite position so its center does not change
	sprPos.x -= sprRadiusDelta;
	sprPos.y -= sprRadiusDelta;
	
	spr->setPos(sprPos);
	spr->setRadius(newRadius);
}

void InGameController::updateHpRecoverySprites()
{
	auto sprIter = m_bonusHpRecoverySprites.begin();

	while (sprIter != m_bonusHpRecoverySprites.end()) {
		auto& spr = *sprIter;

		spr->loopEvent();
		
		if (!spr->isAnimationRunning()) {
			// Animation is finished

			sprIter = m_bonusHpRecoverySprites.erase(sprIter);
		} else {
			// Not finished, move on to the next one

			++sprIter;
		}
	}
}

void InGameController::updateSpritesByAction(
	const std::shared_ptr<CoreAction> action)
{
	switch (action->getType()) {
		case CoreAction::ACTION_NONE:
			updateSpritesByActionNone(action);
			break;
		case CoreAction::ACTION_MULTIPLE:
			updateSpritesByActionMultiple(action);
			break;
		case CoreAction::ACTION_ADD_OBSTACLE:
			updateSpritesByActionAddObstacle(action);
			break;
		case CoreAction::ACTION_SET_STAGE_SIZE:
			updateSpritesByActionSetStageSize(action);
			break;
		case CoreAction::ACTION_ADD_PLAYER:
			updateSpritesByActionAddPlayer(action);
			break;
		case CoreAction::ACTION_REMOVE_PLAYER:
			updateSpritesByActionRemovePlayer(action);
			break;
		case CoreAction::ACTION_SET_PLAYER_POS:
			updateSpritesByActionSetPlayerPos(action);
			break;
		case CoreAction::ACTION_SET_PLAYER_HP:
			updateSpritesByActionSetPlayerHp(action);
			break;
		case CoreAction::ACTION_SET_PLAYER_SIZE:
			updateSpritesByActionSetPlayerSize(action);
			break;
		case CoreAction::ACTION_ADD_BONUS:
			updateSpritesByActionAddBonus(action);
			break;
		case CoreAction::ACTION_REMOVE_BONUS:
			updateSpritesByActionRemoveBonus(action);
			break;
		case CoreAction::ACTION_ANNOUNCE_WINNER:
			updateSpritesByActionAnnounceWinner(action);
			break;
		case CoreAction::ACTION_ANNOUNCE_DRAW_GAME:
			updateSpritesByActionAnnounceDrawGame(action);
			break;
	}
}

void InGameController::updateSpritesByActionNone(
	const std::shared_ptr<CoreAction> action)
{
	(void)action;
	// Nothing to do
}

void InGameController::updateSpritesByActionMultiple(
	const std::shared_ptr<CoreAction> action)
{
	const auto actionCast =
		std::dynamic_pointer_cast<CoreActionMultiple>(action);
	
	for (const auto& a : actionCast->getActions()) {
		updateSpritesByAction(a);
	}
}

void InGameController::updateSpritesByActionAddObstacle(
	const std::shared_ptr<CoreAction> action)
{
	const auto actionCast =
		std::dynamic_pointer_cast<CoreActionAddObstacle>(action);

	createObstacleSprite(actionCast->getShape());
}

void InGameController::updateSpritesByActionSetStageSize(
	const std::shared_ptr<CoreAction> action)
{
	const auto actionCast =
		std::dynamic_pointer_cast<CoreActionSetStageSize>(action);

	Rect stageRect(0, 0, actionCast->getSize());
	RectF holeF = m_viewport->stageToScreen(static_cast<RectF>(stageRect));
	Rect hole = static_cast<Rect>(holeF);
	m_stageBoundsSprite->setHole(hole);
}

void InGameController::updateSpritesByActionAddPlayer(
	const std::shared_ptr<CoreAction> action)
{
	const auto actionCast =
		std::dynamic_pointer_cast<CoreActionAddPlayer>(action);

	createPlayerSprite(actionCast->getId());
	createPlayerHpBgSprite(actionCast->getId());
	createPlayerHpTextSprite(actionCast->getId());
}

void InGameController::updateSpritesByActionRemovePlayer(
	const std::shared_ptr<CoreAction> action)
{
	const auto actionCast =
		std::dynamic_pointer_cast<CoreActionRemovePlayer>(action);
	
	PlayerId id = actionCast->getId();

	m_playerSprites.erase(id);
	m_playerHpBgSprites.erase(id);
	m_playerHpTextSprites.erase(id);
}

void InGameController::updateSpritesByActionSetPlayerPos(
	const std::shared_ptr<CoreAction> action)
{
	const auto actionCast =
		std::dynamic_pointer_cast<CoreActionSetPlayerPos>(action);

	updatePlayerPos(actionCast->getId(), actionCast->getPos());
}

void InGameController::updateSpritesByActionSetPlayerHp(
	const std::shared_ptr<CoreAction> action)
{
	const auto actionCast =
		std::dynamic_pointer_cast<CoreActionSetPlayerHp>(action);

	updatePlayerHp(actionCast->getId(), actionCast->getHp());
}

void InGameController::updateSpritesByActionSetPlayerSize(
	const std::shared_ptr<CoreAction> action)
{
	const auto actionCast =
		std::dynamic_pointer_cast<CoreActionSetPlayerSize>(action);
	
	updatePlayerSize(actionCast->getId(), actionCast->getSize());
}

void InGameController::updateSpritesByActionAddBonus(
	const std::shared_ptr<CoreAction> action)
{
	const auto actionCast =
		std::dynamic_pointer_cast<CoreActionAddBonus>(action);
	
	createBonusSprite(actionCast->getId(), actionCast->getPos());
}

void InGameController::updateSpritesByActionRemoveBonus(
	const std::shared_ptr<CoreAction> action)
{
	const auto actionCast =
		std::dynamic_pointer_cast<CoreActionRemoveBonus>(action);
	
	// Find the collected bonus sprite
	auto bonusSprIter = m_bonusSprites.find(actionCast->getId());

	// Create the HP recovery sprite
	auto recovSpr = std::make_unique<BonusHpRecoverySprite>(sysProxy);
	recovSpr->setBonusRect(bonusSprIter->second->getBounds());
	double hpRecoveryF = actionCast->getHpRecovery() * PLAYER_HP_FACTOR;
	recovSpr->setHpRecovery(static_cast<int>(hpRecoveryF));
	recovSpr->startAnimation();
	// ... and insert it to the set
	m_bonusHpRecoverySprites.insert(std::move(recovSpr));

	// Delete the collected bonus sprite
	m_bonusSprites.erase(bonusSprIter);
}

void InGameController::updateSpritesByActionAnnounceWinner(
	const std::shared_ptr<CoreAction> action)
{
	(void)action;

	m_gameOverTextSprite->setText(GAME_OVER_TEXT_WINNER);
}

void InGameController::updateSpritesByActionAnnounceDrawGame(
	const std::shared_ptr<CoreAction> action)
{
	(void)action;

	m_gameOverTextSprite->setText(GAME_OVER_TEXT_DRAW_GAME);
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

std::shared_ptr<IControllerChild> InGameController::createReplacement()
{
	return ControllerFactory::createMainMenuController(sysProxy);
}

void InGameController::onStarted()
{
	GeneralControllerBase::onStarted();

	initializeViewport();
	createSprites();

	// This will initialize the core
	auto action = m_core->loopEvent();
	updateSpritesByAction(action);
}

void InGameController::onFinished()
{
	m_core->quit();
}

void InGameController::onLoop()
{
	updateHpRecoverySprites();

	auto action = m_core->loopEvent();
	updateSpritesByAction(action);
}

void InGameController::onMouseMove(int x, int y)
{
	checkExitBtnMouseHover(Point(x, y));
}

void InGameController::onMouseBtnDown(MouseBtn btn, int x, int y)
{
	if (btn == BTN_LEFT) {
		checkExitBtnClick(Point(x, y));
	}
}

void InGameController::onPaint(std::shared_ptr<ICanvas> canvas,
	const Rect& invalidRect)
{
	for (auto& [id, spr] : m_playerSprites) {
		spr->repaint(canvas, invalidRect);
	}

	for (auto& [id, spr] : m_bonusSprites) {
		spr->repaint(canvas, invalidRect);
	}

	for (auto& spr : m_obstacleSprites) {
		spr->repaint(canvas, invalidRect);
	}

	m_stageBoundsSprite->repaint(canvas, invalidRect);

	for (auto& spr : m_bonusHpRecoverySprites) {
		spr->repaint(canvas, invalidRect);
	}

	m_gameOverTextSprite->repaint(canvas, invalidRect);

	m_statusBarSprite->repaint(canvas, invalidRect);

	for (auto& [id, spr] : m_playerHpBgSprites) {
		spr->repaint(canvas, invalidRect);
	}

	for (auto& [id, spr] : m_playerHpTextSprites) {
		spr->repaint(canvas, invalidRect);
	}

	m_exitBtnSprite->repaint(canvas, invalidRect);
}
