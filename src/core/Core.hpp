/**
 * @file Core.hpp
 * @author Tomáš Ludrovan
 * @brief Core class
 * @version 0.1
 * @date 2023-12-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef CORE_HPP
#define CORE_HPP

#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "types.hpp"
#include "core/Common.hpp"
#include "core/geometry/Geometry.hpp"
#include "core/stageobstacles/StageObstacles.hpp"
#include "stageserializer/IStageSerializer.hpp"
#include "playerinput/IPlayerInput.hpp"
#include "core/playerstate/PlayerState.hpp"

class Core {
private:
	struct PlayerTurn {
		Trajectory trajectory;
		std::unordered_set<PlayerId> playerCollisions;
	};
	struct TurnData {
		std::unordered_map<PlayerId, PlayerTurn> playerTurns;
	};
	struct PlayerStateInternal {
		Point_2 pos;
		double hp;
		std::shared_ptr<IPlayerInput> input;
	};
private:
	static constexpr double PLAYER_HP_INITIAL = 1.0;
	// HP depleted per frame
	static constexpr double HP_DRAIN = TICK_INTERVAL / 3400.0;
	// Internally, full HP is equal to `1.0`. Externally (as shown to the user),
	// full HP is equal to `100.0`.
	static constexpr double PLAYER_HP_FACTOR = 100.0;

	Timer m_tickTimer;
	std::unordered_map<PlayerId, PlayerStateInternal> m_players;
	std::unique_ptr<StageObstacles> m_stageObstacles;

	void initializeStage(const std::shared_ptr<IStageSerializer> stage,
		const std::vector<std::shared_ptr<IPlayerInput>>& players);
	/**
	 * @brief Game tick event.
	 * 
	 * @details Game tick is the moment when the game state progresses further.
	 *          Players move, bonuses appear, effects are applied, etc.
	 */
	void tick();
	/**
	 * @brief Processes actions of players (movement, bonuses, etc).
	 * 
	 * @note Called every tick.
	 */
	void playersActions();
	void initTurnData(TurnData& turnData);
	void calculateTrajectories(TurnData& turnData);
	void findPlayerCollisions(TurnData& turnData);
	void movePlayers(TurnData& turnData);
	void changePlayersHp(TurnData& turnData);

	double getPlayerSize(PlayerId id) const;
	static double getPlayerSize(double hp);
	double getPlayerSpeed(PlayerId id) const;
	static double getPlayerSpeed(double hp);
	double getPlayerStrength(PlayerId id) const;
	static double getPlayerStrength(double hp);
	void getPlayerMovementVector(PlayerId id, double& x, double& y) const;
	static void getPlayerMovementVector(const PlayerInputFlags& inputFlags,
		double speed, double& x, double& y);
		
	/**
	 * @brief Convert player input value to a vector.
	 * 
	 * @details Based on the player input creates a unit vector in the direction
	 *          the player wants to move.
	 * 
	 * @param input The player input.
	 * @param x Vector X coordinate.
	 * @param y Vector Y coordinate.
	 */
	static void inputToVector(const PlayerInputFlags& input, double& x,
		double& y);
public:
	/**
	 * @brief Constructs a new Core object.
	 * 
	 * @param stage Stage data.
	 * @param players Player input classes. Should map to the players of `stage`
	 *                parameter (stage.players[i] <-> players[i]).
	 */
	Core(const std::shared_ptr<IStageSerializer> stage,
		const std::vector<std::shared_ptr<IPlayerInput>>& players);
	/**
	 * @brief Event that happens every event loop iteration.
	 */
	void loopEvent();
	std::unordered_map<PlayerId, PlayerState> getPlayerStates() const;
	std::vector<StageObstacle> getObstaclesList() const;
	Size2d getStageSize() const;
};

#endif // CORE_HPP
