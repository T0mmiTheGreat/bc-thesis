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
{
	createPlayers();
}

void InGameController::createPlayers()
{
	newPlayer(
		PlayerInputFactory::createKeyboardPlayerInputWSAD(sysProxy),
		200.0, 200.0,
		Color::red()
	);
	newPlayer(
		PlayerInputFactory::createKeyboardPlayerInputArrows(sysProxy),
		400.0, 200.0,
		Color::green()
	);
	newPlayer(
		PlayerInputFactory::createKeyboardPlayerInput(sysProxy,
			KEY_H, KEY_U, KEY_K, KEY_J),
		300.0, 300.0,
		Color(0xff, 0x80, 0xa0)
	);
}

void InGameController::newPlayer(std::shared_ptr<IPlayerInput> playerInput,
	double startX, double startY, const Color& color)
{
	auto playerState = PlayerStateFactory::createDefault(startX, startY,
		playerInput);
	m_core->addPlayer(playerState);

	auto playerSprite = std::make_unique<PlayerSprite>(sysProxy);
	playerSprite->setColor(color);
	m_playerSprites.push_back(std::move(playerSprite));
}

void InGameController::updatePlayerSprites()
{
	auto plStates = m_core->getPlayerList();

	for (int i = 0; i < PLAYER_COUNT; i++) {
		m_playerSprites[i]->setPos(plStates[i]->getX(), plStates[i]->getY());
		m_playerSprites[i]->setRadius(static_cast<int>(plStates[i]->getSize()));
	}
}

void InGameController::onActivated()
{
	GeneralControllerBase::onActivated();
	m_core->loopEvent();
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
}
