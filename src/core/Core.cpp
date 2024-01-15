/**
 * @file Core.cpp
 * @author Tomáš Ludrovan
 * @brief Core class
 * @version 0.1
 * @date 2023-12-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "core/Core.hpp"

#include "core/trajectory/Trajectory.hpp"
#include "math/Math.hpp"

Core::Core()
	: m_tickTimer(TICK_INTERVAL)
{}

void Core::initTurnData(TurnData& turnData)
{
	turnData.playerTurns.reserve(m_players.size());
	
	for (auto playerRef : m_players) {
		turnData.playerTurns.push_back(PlayerTurn{
			.playerRef = playerRef,
		});
	}
}

void Core::calculateTrajectories(TurnData& turnData)
{
	for (auto& playerTurn : turnData.playerTurns) {
		m_obstacles.getPlayerTrajectory(playerTurn.playerRef, playerTurn.trajectory);
	}
}

void Core::findPlayerCollisions(TurnData& turnData)
{
	for (auto& playerTurnA : turnData.playerTurns) {
		for (auto& playerTurnB : turnData.playerTurns) {
			if (std::addressof(playerTurnA) != std::addressof(playerTurnB)) {
				double minSqdist = playerTurnA.trajectory.minSqdist(playerTurnB.trajectory);
				double playerSqsizes = sqr(playerTurnA.playerRef->getSize() + playerTurnB.playerRef->getSize());
				if (minSqdist <= playerSqsizes) {
					playerTurnA.playerCollisions.push_back(playerTurnB.playerRef);
				}
			}
		}
	}
}

void Core::movePlayers(TurnData& turnData)
{
	double x, y;

	for (auto& playerTurn : turnData.playerTurns) {
		playerTurn.trajectory.getDelta(x, y);
		playerTurn.playerRef->incPos(x, y);
	}
}

void Core::changePlayersHp(TurnData& turnData)
{
	for (auto& playerTurn : turnData.playerTurns) {
		playerTurn.playerRef->incHp(-HP_DRAIN*playerTurn.playerCollisions.size());
	}
}

void Core::tick()
{
	playersActions();
}

void Core::playersActions()
{
	TurnData turnData;

	initTurnData(turnData);
	calculateTrajectories(turnData);
	findPlayerCollisions(turnData);
	movePlayers(turnData);
	changePlayersHp(turnData);
}

void Core::loopEvent()
{
	if (m_tickTimer.isLap()) {
		tick();
	}
}

void Core::addPlayer(std::shared_ptr<IPlayerState> player)
{
	m_players.push_back(player);
}

const PlayerList& Core::getPlayerList()
{
	return m_players;
}
