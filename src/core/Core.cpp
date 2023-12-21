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

void Core::playerTick(std::shared_ptr<IPlayerState> player)
{
	auto playerInput = player->getPlayerInputObject()->readInput();
	double vecX, vecY;
	inputToVector(playerInput, vecX, vecY);
	player->deltaPos(vecX * player->getSpeed(), vecY * player->getSpeed());
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
	for (auto player : m_players) {
		playerTick(player);
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
