/**
 * @file GameStateAgentProxy.hpp
 * @author Tomáš Ludrovan
 * @brief GameStateAgentProxy class
 * @version 0.1
 * @date 2024-03-26
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef GAMESTATEAGENTPROXY_HPP
#define GAMESTATEAGENTPROXY_HPP

#include <memory>

#include "types.hpp"

/**
 * @brief A proxy directing access to the game state for AI player agents.
 */
class GameStateAgentProxy {
public:
	struct PlayerState {
		PointF pos;      // Player's position
		double hp;       // Player's health
		double strength; // Player's strength (damage dealt per ms).
	};

	typedef std::vector<PlayerState> PlayerStateCollection;
public:
	virtual ~GameStateAgentProxy() {}
	/**
	 * @brief Returns a collection of player states.
	 */
	virtual PlayerStateCollection getPlayers() = 0;
};

typedef std::shared_ptr<GameStateAgentProxy> GameStateAgentProxyP;

#endif // GAMESTATEAGENTPROXY_HPP
