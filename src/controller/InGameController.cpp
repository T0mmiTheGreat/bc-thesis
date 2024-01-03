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

InGameController::InGameController()
	: GeneralControllerBase()
	, m_core{std::make_unique<Core>()}
	, m_tickTimer(TICK_TIMER_INTERVAL)
{
	auto player1Input = PlayerInputFactory::createKeyboardPlayerInputWSAD();
	auto player1 = PlayerStateFactory::createDefault(200.0, 200.0, player1Input);
	
	auto player2Input = PlayerInputFactory::createKeyboardPlayerInputArrows();
	auto player2 = PlayerStateFactory::createDefault(400.0, 200.0, player2Input);

	m_core->addPlayer(player1);
	m_core->addPlayer(player2);
}

void InGameController::startedEvent()
{
	m_tickTimer.reset();
}

void InGameController::loopEvent()
{
	if (m_tickTimer.isLap()) {
		m_core->tick();
		sysProxy->invalidateRect();
	}
}

void InGameController::paintEvent(std::shared_ptr<ICanvas> canvas, Rect & invalidRect)
{
	for (auto player : m_core->getPlayerList()) {
		canvas->setFillingColor(Color::red());
		canvas->fillCircle(
			static_cast<int>(player->getX()),
			static_cast<int>(player->getY()),
			static_cast<int>(player->getSize())
		);
	}
}
