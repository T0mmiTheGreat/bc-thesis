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
#include <unordered_map>

#include "types.hpp"
#include "core/Common.hpp"
#include "core/stageobstacles/StageObstacles.hpp"

/**
 * @brief A proxy directing access to the game state for AI player agents.
 */
class GameStateAgentProxy {
public:
	struct PlayerState {
		PointF pos;      // Player's position
		double hp;       // Player's health
		double speed;    // Player's speed (steps per ms)
		double strength; // Player's strength (damage dealt per ms).
	};

	typedef std::unordered_map<PlayerId, PlayerState> PlayerStateCollection;
public:
	virtual ~GameStateAgentProxy() {}
	/**
	 * @brief Returns a collection of player states.
	 */
	virtual const PlayerStateCollection& getPlayers() const = 0;
	virtual const StageObstacles& getObstacles() const = 0;
};

typedef std::shared_ptr<GameStateAgentProxy> GameStateAgentProxyP;

#endif // GAMESTATEAGENTPROXY_HPP
