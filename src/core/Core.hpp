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
#include "aiplayeragent/GameStateAgentProxy.hpp"
#include "aiplayeragent/IAIPlayerAgent.hpp"
#include "core/Common.hpp"
#include "core/CoreAction.hpp"
#include "core/bonuseffect/BonusEffect.hpp"
#include "core/bonuseffect/EffectAttributes.hpp"
#include "core/geometry/Geometry.hpp"
#include "core/playerstate/PlayerState.hpp"
#include "core/stagebonuses/StageBonuses.hpp"
#include "core/stageobstacles/StageObstacles.hpp"
#include "gamesetupdata/GameSetupData.hpp"
#include "playerinput/IPlayerInput.hpp"
#include "stageserializer/IStageSerializer.hpp"

class Core {
private:
	struct PlayerStateInternal {
		Point_2 pos;
		double hp;
		std::unordered_set<std::shared_ptr<BonusEffect>> bonusEffects;
		std::shared_ptr<IPlayerInput> input;
	};
	struct PlayerCollision {
		double opponentStrength;
	};
	struct BonusCollision {
		BonusId id;
	};
	struct PlayerTurn {
		PlayerStateInternal* playerRef;
		Trajectory trajectory;
		std::vector<PlayerCollision> playerCollisions;
		std::vector<BonusCollision> bonusCollisions;
		EffectAttributes effectAttributes;
	};
	struct TurnData {
		std::unordered_map<PlayerId, PlayerTurn> playerTurns;
		std::unordered_set<BonusId> collectedBonuses;
	};

	friend class GameStateAgentProxyImplem;
	class GameStateAgentProxyImplem : public GameStateAgentProxy {
	private:
		const Core& m_core;
		PlayerStateCollection m_players;
	public:
		GameStateAgentProxyImplem(const Core& core)
			: m_core{core}
		{}

		void update() {
			// Update players
			for (const auto& [id, playerState] : m_core.m_players) {
				auto& playerRef = m_players[id];

				playerRef.pos      = playerState.pos;
				playerRef.hp       = playerState.hp;
				playerRef.speed    = Core::getPlayerSpeed(playerState.hp);
				playerRef.strength = Core::getPlayerStrength(playerState.hp);
				playerRef.size     = Core::getPlayerSize(playerState.hp);
			}
		}

		const PlayerStateCollection& getPlayers() const override {
			return m_players;
		}

		const StageObstacles& getObstacles() const override {
			return *m_core.m_stageObstacles;
		}
	};
	
private:
	static constexpr double PLAYER_HP_INITIAL = 1.0;
	// Strength (damage dealt per millisecond) of a player with full HP
	static constexpr double BASE_STRENGTH = 1.0/3400.0;
	// HP decrement of a "deflate" action per game tick
	static constexpr double DEFLATE_AMOUNT = BASE_STRENGTH * TICK_INTERVAL;

	// Has the `initializeStage()` method been called yet?
	bool m_isInitialized;
	// Is the game over?
	bool m_isOver;
	GameSetupData m_gsdata;

	Timer m_tickTimer;
	std::unordered_map<PlayerId, PlayerStateInternal> m_players;
	std::vector<std::shared_ptr<IAIPlayerAgent>> m_aiAgents;
	std::unique_ptr<StageObstacles> m_stageObstacles;
	std::unique_ptr<StageBonuses> m_stageBonuses;

	std::shared_ptr<GameStateAgentProxyImplem> m_gsAgentProxy;

	Timer m_bonusTimer;

	/**
	 * @brief Initializes the internal stage state.
	 * 
	 * @return The action performed.
	 */
	CoreActionPtr initializeStage();
	CoreActionPtr initializeStagePlayers();
	CoreActionPtr initializeStageObstaclesAndBounds();
	CoreActionPtr initializeStageBonuses();
	CoreActionPtr initializeStageAiAgents();
	/**
	 * @brief Game tick event.
	 * 
	 * @details Game tick is the moment when the game state progresses further.
	 *          Players move, bonuses appear, effects are applied, etc.
	 * 
	 * @return The action performed.
	 */
	CoreActionPtr tick();
	/**
	 * @brief Processes actions of players (movement, bonuses, etc).
	 * 
	 * @note Called every tick.
	 * 
	 * @return The action performed.
	 */
	CoreActionPtr playersActions();
	/**
	 * @brief Initializes turn data structure.
	 * 
	 * @return The action performed.
	 */
	CoreActionPtr initTurnData(TurnData& turnData);
	/**
	 * @brief Applies active bonus effects on players.
	 * 
	 * @return The action performed.
	 */
	CoreActionPtr applyPlayerEffects(TurnData& turnData);
	/**
	 * @brief Calculates trajectories of players' movement.
	 * 
	 * @return The action performed.
	 */
	CoreActionPtr calculateTrajectories(TurnData& turnData);
	/**
	 * @brief Finds collisions of players with other players and bonuses.
	 * 
	 * @return The action performed.
	 */
	CoreActionPtr findPlayerAndBonusCollisions(TurnData& turnData);
	/**
	 * @brief Updates the players' position, healts, and active bonus effects.
	 * 
	 * @return The action performed.
	 */
	CoreActionPtr updatePlayersStates(TurnData& turnData);
	/**
	 * @brief Check if there is less than two players alive.
	 * 
	 * @return The action performed.
	 */
	CoreActionPtr checkIsGameOver(TurnData& turnData);
	/**
	 * @brief Removes the collected bonuses.
	 * 
	 * @return The action performed.
	 */
	CoreActionPtr clearBonuses(TurnData& turnData);
	/**
	 * @brief Attempts to generate new bonuses.
	 * 
	 * @return The action performed.
	 */
	CoreActionPtr generateBonus(TurnData& turnData);
	
	/**
	 * @brief Finds collisions of the given player and other players.
	 * 
	 * @return The action performed.
	 */
	CoreActionPtr findPlayerPlayerCollisions(PlayerId id,
		PlayerTurn& playerTurn, TurnData& turnData);
	/**
	 * @brief Finds collisions of the given player and bonuses.
	 * 
	 * @return The action performed.
	 */
	CoreActionPtr findPlayerBonusCollisions(PlayerId id,
		PlayerTurn& playerTurn, TurnData& turnData);
	/**
	 * @brief Updates the position of the given player.
	 * 
	 * @return The action performed.
	 */
	CoreActionPtr movePlayer(PlayerId id,
		PlayerTurn& playerTurn, TurnData& turnData);
	/**
	 * @brief Updates the health points of the given player.
	 * 
	 * @return The action performed.
	 */
	CoreActionPtr changePlayerHp(PlayerId id,
		PlayerTurn& playerTurn, TurnData& turnData);
	/**
	 * @brief Updates the active bonus effects of the given player.
	 * 
	 * @return The action performed.
	 */
	CoreActionPtr applyPlayerBonusCollisions(PlayerId id,
		PlayerTurn& playerTurn, TurnData& turnData);

	static Timer createNewBonusTimer();
	void resetBonusTimer();

	void notifyAgents();

	/**
	 * @brief Calculates the size (radius) of the player based on their HP.
	 */
	double getPlayerSize(PlayerId id) const;
	/**
	 * @brief Calculates the size (radius) of the player based on their HP.
	 */
	static double getPlayerSize(double hp);
	/**
	 * @brief Calculates the speed (steps per millisecond) of the player based
	 *        on their HP.
	 */
	double getPlayerSpeed(PlayerId id) const;
	/**
	 * @brief Calculates the speed (steps per millisecond) of the player based
	 *        on their HP.
	 */
	static double getPlayerSpeed(double hp);
	/**
	 * @brief Calculates the strength (damage dealt per millisecond) of the
	 *        player based on their HP.
	 */
	double getPlayerStrength(PlayerId id) const;
	/**
	 * @brief Calculates the strength (damage dealt per millisecond) of the
	 *        player based on their HP.
	 */
	static double getPlayerStrength(double hp);
	/**
	 * @brief Calculates the increment in X and Y coordinate of a player based
	 *        on their input and speed.
	 */
	void getPlayerMovementVector(PlayerId id, double& x, double& y) const;
	/**
	 * @brief Calculates the increment in X and Y coordinate of a player based
	 *        on their input and speed.
	 */
	static void getPlayerMovementVector(const PlayerInputFlags& inputFlags,
		double speed, double& x, double& y);
public:
	/**
	 * @brief Constructs a new Core object.
	 */
	Core(const GameSetupData& gsdata);
	/**
	 * @brief Quits the core.
	 * 
	 * @details This method must be called before calling the object destructor,
	 *          exactly once. 
	 */
	void quit();
	/**
	 * @brief Event that happens every event loop iteration.
	 * 
	 * @return The action performed.
	 */
	const CoreActionPtr loopEvent();
	std::unordered_map<PlayerId, PlayerState> getPlayerStates() const;
	std::vector<StageObstacle> getObstaclesList() const;
	Size2d getStageSize() const;
};

#endif // CORE_HPP
