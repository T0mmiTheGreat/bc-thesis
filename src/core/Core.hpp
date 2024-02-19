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
#include <vector>

#include "types.hpp"
#include "core/Common.hpp"
#include "core/stageobstacles/StageObstacles.hpp"
#include "stageserializer/IStageSerializer.hpp"
#include "playerinput/IPlayerInput.hpp"
#include "playerstate/IPlayerState.hpp"

typedef std::vector<std::shared_ptr<IPlayerState>> PlayerList;

class Core {
private:
	struct PlayerTurn {
		std::shared_ptr<IPlayerState> playerRef;
		Trajectory trajectory;
		// This could also use unordered_set, but that's probably too complex;
		// the operations needed are add(), read(), and clear(), each in random
		// order, so vector is just enough, plus it has less overhead
		std::vector<std::shared_ptr<IPlayerState>> playerCollisions;
	};
	struct TurnData {
		std::vector<PlayerTurn> playerTurns;
	};
private:
	// HP depleted per frame
	static constexpr double HP_DRAIN = TICK_INTERVAL / 3400.0;

	/**
	 * @brief Player list.
	 */
	PlayerList m_players;
	Timer m_tickTimer;
	std::unique_ptr<StageObstacles> m_obstacles;

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
public:
	Core(const std::shared_ptr<IStageSerializer> stage,
		const std::vector<std::shared_ptr<IPlayerInput>>& players);
	/**
	 * @brief Event that happens every event loop iteration.
	 */
	void loopEvent();
	/**
	 * @brief Access the player list.
	 */
	const PlayerList& getPlayerList() const;
	const std::vector<StageObstacle>& getObstaclesList() const;
};

#endif // CORE_HPP
