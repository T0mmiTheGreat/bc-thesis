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

#include "core/geometry/Geometry.hpp"
#include "core/stageobstacles/StageObstacles.hpp"
#include "playerinput/PlayerInputFlags.hpp"

/**
 * @brief A proxy directing access to the game state for AI player agents.
 */
class GameStateAgentProxy {
public:
	struct PlayerState {
		Point_2 pos;     // Player's position
		double hp;       // Player's health
		double speed;    // Player's speed (steps per ms)
		double strength; // Player's strength (damage dealt per ms).
		double size;     // Player's radius
	};

	typedef std::unordered_map<PlayerId, PlayerState> PlayerStateCollection;
public:
	virtual ~GameStateAgentProxy() {}
	/**
	 * @brief Returns a collection of player states.
	 */
	virtual const PlayerStateCollection& getPlayers() const = 0;
	/**
	 * @brief Returns an object representing the obstacles on the stage.
	 */
	virtual const StageObstacles& getObstacles() const = 0;
	/**
	 * @brief Calculates the increment in X and Y coordinate of a player based
	 *        on their input.
	 * 
	 * @param input Desired player input.
	 * @param playerId ID of the player for which the increment should be
	 *                 calculated.
	 * @param x X increment.
	 * @param y Y increment.
	 */
	virtual void getPlayerMovementVector(const PlayerInputFlags& input,
		PlayerId playerId, double& x, double& y) const = 0;
	/**
	 * @brief Calculates the increment in X and Y coordinate of a player based
	 *        on their input.
	 * 
	 * @param input Desired player input.
	 * @param ps State of the player for which the increment should be
	 *           calculated.
	 * @param x X increment.
	 * @param y Y increment.
	 */
	virtual void getPlayerMovementVector(const PlayerInputFlags& input,
		const PlayerState& ps, double& x, double& y) const = 0;
	/**
	 * @brief Calculates new player position based on their input taking the
	 *        obstacles into account.
	 * 
	 * @param currPos Position from which the new position should be calculated.
	 * @param input Desired player input.
	 * @param playerId ID of the player for which the new position should be
	 *                 calculated.
	 * @return 
	 */
	virtual Point_2 calculateNewPlayerPos(const Point_2& currPos,
		const PlayerInputFlags& input, PlayerId playerId) const = 0;
	/**
	 * @brief Calculates new player position based on their input taking the
	 *        obstacles into account.
	 * 
	 * @param currPos Position from which the new position should be calculated.
	 * @param input Desired player input.
	 * @param ps State of the player for which the new position should be
	 *           calculated.
	 */
	virtual Point_2 calculateNewPlayerPos(const Point_2& currPos,
		const PlayerInputFlags& input, const PlayerState& ps) const = 0;
};

typedef std::shared_ptr<GameStateAgentProxy> GameStateAgentProxyP;

#endif // GAMESTATEAGENTPROXY_HPP
