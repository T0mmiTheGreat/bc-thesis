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
#include "playerstate/PlayerStateFactory.hpp"

InGameController::InGameController(std::shared_ptr<ISysProxy> sysProxy,
	const std::shared_ptr<IStageSerializer> stage,
	const std::vector<std::shared_ptr<IPlayerInput>>& players)
	: GeneralControllerBase(sysProxy)
	, m_core{std::make_unique<Core>(stage, players)}
{}

void InGameController::createPlayerSprites()
{
	const auto& players = m_core->getPlayerList();
	for (size_t i = 0; i < players.size(); i++) {
		auto playerSprite = std::make_unique<PlayerSprite>(sysProxy);
		playerSprite->setColor(Color::player(i));
		m_playerSprites.push_back(std::move(playerSprite));
	}
}

void InGameController::createObstacleSprites()
{
	for (const auto& obstacle : m_core->getObstaclesList()) {
		auto obstacleSprite = std::make_unique<ObstacleSprite>(sysProxy);
		obstacleSprite->setShape(obstacle);
		m_obstacleSprites.push_back(std::move(obstacleSprite));
	}
}

void InGameController::updatePlayerSprites()
{
	auto plStates = m_core->getPlayerList();

	for (size_t i = 0; i < m_playerSprites.size(); i++) {
		m_playerSprites[i]->setPos(plStates[i]->getX(), plStates[i]->getY());
		m_playerSprites[i]->setRadius(static_cast<int>(plStates[i]->getSize()));
	}
}

void InGameController::onStarted()
{
	GeneralControllerBase::onStarted();
	m_core->loopEvent();
	createPlayerSprites();
	createObstacleSprites();
	updatePlayerSprites();
}

void InGameController::onLoop()
{
	m_core->loopEvent();
	updatePlayerSprites();
}

void InGameController::onPaint(std::shared_ptr<ICanvas> canvas,
	const Rect& invalidRect)
{
	for (auto& spr : m_playerSprites) {
		spr->repaint(canvas, invalidRect);
	}

	for (auto& spr : m_obstacleSprites) {
		spr->repaint(canvas, invalidRect);
	}
}
