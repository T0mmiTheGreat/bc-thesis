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
#include "playerstate/IPlayerState.hpp"
#include "playerstate/PlayerStateFactory.hpp"

Core::Core(const std::shared_ptr<IStageSerializer> stage,
	const std::vector<std::shared_ptr<IPlayerInput>>& players)
	: m_tickTimer(TICK_INTERVAL)
{
	initializeStage(stage, players);
}

void Core::initTurnData(TurnData& turnData)
{
	turnData.playerTurns.reserve(m_players.size());
	
	for (auto playerRef : m_players) {
		turnData.playerTurns.push_back(PlayerTurn{
			.playerRef = playerRef,
			.trajectory = Trajectory(),
			.playerCollisions = PlayerList(),
		});
	}
}

void Core::calculateTrajectories(TurnData& turnData)
{
	for (auto& playerTurn : turnData.playerTurns) {
		m_obstacles->getPlayerTrajectory(playerTurn.playerRef, playerTurn.trajectory);
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

void Core::initializeStage(const std::shared_ptr<IStageSerializer> stage,
	const std::vector<std::shared_ptr<IPlayerInput>>& players)
{
	assert(players.size() <= stage->getPlayers().size());

	std::shared_ptr<IPlayerState> newPlayer;

	// Players
	for (size_t i = 0; i < players.size(); i++) {
		const auto& playerPos = stage->getPlayers()[i];
		newPlayer = PlayerStateFactory::createDefault(playerPos.x, playerPos.y,
			players[i]);
		m_players.push_back(newPlayer);
	}

	// Obstacles
	m_obstacles = std::make_unique<StageObstacles>(stage->getObstacles());
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

const PlayerList& Core::getPlayerList() const
{
	return m_players;
}

const std::vector<StageObstacle>& Core::getObstaclesList() const
{
	return m_obstacles->getObstaclesList();
}
