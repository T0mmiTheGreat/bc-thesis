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

InGameController::InGameController(std::shared_ptr<ISysProxy> sysProxy)
	: GeneralControllerBase(sysProxy)
	, m_core{std::make_unique<Core>()}
	, m_tickTimer(ICore::TICK_INTERVAL)
{
	auto player1Input = PlayerInputFactory::createKeyboardPlayerInputWSAD(sysProxy);
	auto player1State = PlayerStateFactory::createDefault(200.0, 200.0, player1Input);
	auto player1Sprite = std::make_unique<PlayerSprite>(sysProxy);
	
	auto player2Input = PlayerInputFactory::createKeyboardPlayerInputArrows(sysProxy);
	auto player2State = PlayerStateFactory::createDefault(400.0, 200.0, player2Input);
	auto player2Sprite = std::make_unique<PlayerSprite>(sysProxy);

	m_core->addPlayer(player1State);
	m_core->addPlayer(player2State);

	player1Sprite->setColor(Color::red());
	player2Sprite->setColor(Color::green());

	m_playerSprites.push_back(std::move(player1Sprite));
	m_playerSprites.push_back(std::move(player2Sprite));
}

void InGameController::updatePlayerSprites()
{
	auto plStates = m_core->getPlayerList();

	for (int i = 0; i < PLAYER_COUNT; i++) {
		m_playerSprites[i]->setPos(plStates[i]->getX(), plStates[i]->getY());
		m_playerSprites[i]->setRadius(static_cast<int>(plStates[i]->getSize()));
	}
}

void InGameController::startedEvent()
{
	m_tickTimer.reset();
	updatePlayerSprites();
}

void InGameController::loopEvent()
{
	if (m_tickTimer.isLap()) {
		m_core->tick();
		updatePlayerSprites();
	}
}

void InGameController::paintEvent(std::shared_ptr<ICanvas> canvas, Rect& invalidRect)
{
	for (auto& spr : m_playerSprites) {
		spr->repaint(canvas, invalidRect);
	}
}
