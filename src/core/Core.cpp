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

#include "functions.hpp"

void Core::playerTick(std::shared_ptr<IPlayerState> player)
{
	auto playerInput = player->getPlayerInputObject()->readInput();
	double vecX, vecY;
	inputToVector(playerInput, vecX, vecY);
	player->deltaPos(vecX * player->getSpeed() * SPEED_FACTOR, vecY * player->getSpeed() * SPEED_FACTOR);
}

void Core::inputToVector(const PlayerInputState& input, double& x, double& y)
{
	// If none or both keys for horizontal movement are pressed, the player
	// won't move horizontally
	bool isHorzMove = (input.left != input.right);
	// Same as isHorzMove
	bool isVertMove = (input.up != input.down);

	if (!isHorzMove && !isVertMove) {
		x = 0.0;
		y = 0.0;
	} else if (isHorzMove && isVertMove) {
		// Diagonal

		x = (input.left ? -DIAG_MOVEMENT_PERAXIS_LENGTH : DIAG_MOVEMENT_PERAXIS_LENGTH);
		y = (input.up ? -DIAG_MOVEMENT_PERAXIS_LENGTH : DIAG_MOVEMENT_PERAXIS_LENGTH);
	} else {
		// Orthogonal
		
		if (isHorzMove) {
			x = (input.left ? -1.0 : 1.0);
			y = 0.0;
		} else {
			x = 0.0;
			y = (input.up ? -1.0 : 1.0);
		}
	}
}

void Core::tick()
{
	static constexpr double hpDrain = TICK_INTERVAL / 3400.0;

	for (auto player : m_players) {
		playerTick(player);
	}

	for (int i = 0; i < m_players.size(); i++) {
		for (int j = i + 1; j < m_players.size(); j++) {
			auto plA = m_players[i];
			auto plB = m_players[j];
			if (distance(plA->getX(), plA->getY(), plB->getX(), plB->getY()) <= plA->getSize() + plB->getSize()) {
				plA->setHp(plA->getHp() - hpDrain);
				plB->setHp(plB->getHp() - hpDrain);
			}
		}
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
