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
#include "core/trajectory/Trajectory.hpp"
#include "math/Math.hpp"

#ifdef INCLUDE_BENCHMARK
#include "utilities/benchmark/Benchmark.hpp"

static constexpr const char* BENCH_ID_PL_ACTIONS = "core-pl-actions";
static constexpr const char* BENCH_ID_UPDATE_GSPROXY = "core-update-gsproxy";
#endif // INCLUDE_BENCHMARK

Core::Core(const GameSetupData& gsdata)
	: m_isInitialized{false}
	, m_isOver{false}
	, m_gsdata{gsdata}
	, m_tickTimer(TICK_INTERVAL)
	, m_bonusTimer{createNewBonusTimer()}
{
	assert(gsdata.players.size() <= gsdata.stage->getPlayers().size());
}

CoreActionPtr Core::initTurnData(TurnData& turnData)
{
	turnData.playerTurns.reserve(m_players.size());
	
	for (auto& [id, state] : m_players) {
		turnData.playerTurns[id] = PlayerTurn{
			&state, // playerRef
#ifndef OLD_TRAJECTORY_ALGORITHM
			Trajectory(state.pos), // trajectory
#else
			Trajectory(), // trajectory
#endif
			std::vector<PlayerCollision>(), // playerCollisions
			std::vector<BonusCollision>(), // bonusCollisions
			EffectAttributes(), // effectAttributes
		};
	}

	return std::make_shared<CoreActionNone>();
}

CoreActionPtr Core::applyPlayerEffects(TurnData& turnData)
{
	for (auto& [id, turn] : turnData.playerTurns) {
		auto& player = *turn.playerRef;

		// For each active bonus effect...
		auto iter = player.bonusEffects.begin();
		while (iter != player.bonusEffects.end()) {
			// Access the effect object through iterator
			auto& effect = *iter;
			
			// Apply
			effect->applyEffect(turn.effectAttributes);

			if (effect->isActive()) {
				// Still active -- move on to the next one

				iter++;
			} else {
				// The effect has run out -- erase it and then move on to the
				// next one

				// Note: `erase()` returns the iterator following the removed
				// element
				iter = player.bonusEffects.erase(iter);
			}
		}
	}

	return std::make_shared<CoreActionNone>();
}

CoreActionPtr Core::calculateTrajectories(TurnData& turnData)
{
	double vx, vy;
	Point_2 source;
	Vector_2 v;
	
	for (auto& [id, playerTurn] : turnData.playerTurns) {

		getPlayerMovementVector(id, vx, vy);
		source = m_players[id].pos;
		v = Vector_2(vx, vy);

		playerTurn.trajectory = m_stageObstacles->getPlayerTrajectory(source,
			v, getPlayerSize(id));
	}

	return std::make_shared<CoreActionNone>();
}

CoreActionPtr Core::findPlayerAndBonusCollisions(
	TurnData& turnData)
{
	CoreActionMultiple::ActionsCollection actionsGroup;
	CoreActionPtr actionFindPlayerPlayerCollisions,
		actionFindPlayerBonusCollisions;

	for (auto& [id, playerTurn] : turnData.playerTurns) {
		actionFindPlayerPlayerCollisions = findPlayerPlayerCollisions(id,
			playerTurn, turnData);
		actionFindPlayerBonusCollisions = findPlayerBonusCollisions(id,
			playerTurn, turnData);
		
		// Add to actions list
		actionsGroup.push_back(actionFindPlayerPlayerCollisions);
		actionsGroup.push_back(actionFindPlayerBonusCollisions);
	}

	// Merge
	auto res = CoreActionMultiple::getMergedActions(actionsGroup);

	return res;
}

CoreActionPtr Core::updatePlayersStates(TurnData& turnData)
{
	CoreActionMultiple::ActionsCollection actionsGroup;
	CoreActionPtr actionMovePlayer, actionChangePlayerHp,
		actionApplyPlayerBonusCollisions;
	
	for (auto& [id, playerTurn] : turnData.playerTurns) {
		actionMovePlayer = movePlayer(id, playerTurn, turnData);
		actionChangePlayerHp = changePlayerHp(id, playerTurn, turnData);
		actionApplyPlayerBonusCollisions = applyPlayerBonusCollisions(id,
			playerTurn, turnData);
		
		// Add to actions list
		actionsGroup.push_back(actionMovePlayer);
		actionsGroup.push_back(actionChangePlayerHp);
		actionsGroup.push_back(actionApplyPlayerBonusCollisions);
	}

	// Merge
	auto res = CoreActionMultiple::getMergedActions(actionsGroup);

	return res;
}

CoreActionPtr Core::checkIsGameOver(TurnData& turnData)
{
	(void)turnData;

	if (m_players.size() == 0) {
		// Draw game

		auto res = std::make_shared<CoreActionAnnounceDrawGame>();
		return res;
	}
	else if (m_players.size() == 1) {
		// Winner

		auto iter = m_players.cbegin();
		PlayerId id = iter->first;

		auto res = std::make_shared<CoreActionAnnounceWinner>(id);
		return res;
	}
	else {
		return std::make_shared<CoreActionNone>();
	}
}

CoreActionPtr Core::clearBonuses(TurnData& turnData)
{
	std::vector<CoreActionPtr> actionsGroup;
	std::shared_ptr<CoreActionRemoveBonus> actionRemoveBonus;

	for (BonusId id : turnData.collectedBonuses) {
		double hpRecovery = m_stageBonuses->getBonusHpRecovery(id);

		// Clear
		m_stageBonuses->clearBonus(id);

		// Reset timer
		resetBonusTimer();

		// Add to actions list
		actionRemoveBonus = std::make_shared<CoreActionRemoveBonus>(id,
			hpRecovery);
		actionsGroup.push_back(std::move(actionRemoveBonus));
	}

	// Merge
	auto res = CoreActionMultiple::getMergedActions(actionsGroup);

	return res;
}

CoreActionPtr Core::generateBonus(TurnData& turnData)
{
	(void)turnData;

	if (m_stageBonuses->canGenerateBonus()) {
		if (m_bonusTimer.isLap()) {
#ifdef ENABLE_BONUS_CONSTRAINTS
			auto playerStatesMap = getPlayerStates();
			std::vector<PlayerState> playerStatesVec;
			playerStatesVec.reserve(playerStatesMap.size());
			for (const auto& [id, state] : playerStatesMap) {
				playerStatesVec.push_back(state);
			}
			m_stageBonuses->reportPlayerStates(playerStatesVec);
#endif // ENABLE_BONUS_CONSTRAINTS

			BonusId bonusId = m_stageBonuses->generateBonus();
			if (bonusId == BONUS_ID_NULL) {
				// Bonus could not be generated
				
				return std::make_shared<CoreActionNone>();
			} else {
				const PointF& bonusPos = m_stageBonuses->getBonuses().at(bonusId).position;

				auto res = std::make_shared<CoreActionAddBonus>(bonusId, bonusPos);
				return res;
			}
		}
	}
	
	// Else
	return std::make_shared<CoreActionNone>();
}

CoreActionPtr Core::findPlayerPlayerCollisions(PlayerId id,
	PlayerTurn& playerTurn, TurnData& turnData)
{
	// Aliases
	PlayerId& idA = id;
	PlayerTurn& playerTurnA = playerTurn;

	for (const auto& [idB, playerTurnB] : turnData.playerTurns) {
		if (idA == idB) continue; // A player cannot collide with itself

		// Minimum distance between players' trajectories
		double minSqdist = playerTurnA.trajectory.minSqdist(
			playerTurnB.trajectory);
		// Square of sum of the players' sizes
		double playerSqsizes = sqr(getPlayerSize(idA) + getPlayerSize(idB));

		if (minSqdist <= playerSqsizes) {
			// Add collision with B to A's player collisions
			PlayerCollision collision = {
				getPlayerStrength(idB) // opponentStrength
			};
			playerTurnA.playerCollisions.push_back(collision);
		}
	}

	return std::make_shared<CoreActionNone>();
}

CoreActionPtr Core::findPlayerBonusCollisions(PlayerId id,
	PlayerTurn& playerTurn, TurnData& turnData)
{
	// Alias
	PlayerId& playerId = id;

	for (const auto& [bonusId, bonusData] : m_stageBonuses->getBonuses()) {
		// Create zero length trajectory representing the movement of the
		// bonus (it doesn't move, but we need the trajectory).
		Trajectory bonusTraj(toCgalPoint(bonusData.position));

		// Minimum distance between the player's and bonus's trajectories
		double minSqdist = playerTurn.trajectory.minSqdist(bonusTraj);
		// Square of sum of the player's and bonus's size
		double sqSizes = sqr(getPlayerSize(playerId) + BONUS_RADIUS);

		if (minSqdist <= sqSizes) {
			// Add the collision to the player's bonus collisions
			BonusCollision collision = {bonusId};
			playerTurn.bonusCollisions.push_back(std::move(collision));

			// Add the collision to turn data
			turnData.collectedBonuses.insert(bonusId);
		}
	}

	return std::make_shared<CoreActionNone>();
}

CoreActionPtr Core::movePlayer(PlayerId id,
	PlayerTurn& playerTurn, TurnData& turnData)
{
	(void)turnData;

	// Alias
	auto& player = *playerTurn.playerRef;

	// Move player
#ifndef OLD_TRAJECTORY_ALGORITHM
	player.pos = playerTurn.trajectory.end();
#else
	player.pos = playerTurn.trajectory.last().getPEnd();
#endif

	// Create action
	auto res = std::make_shared<CoreActionSetPlayerPos>(id,
		fromCgalPoint(player.pos));
	
	return res;
}

CoreActionPtr Core::changePlayerHp(PlayerId id,
	PlayerTurn& playerTurn, TurnData& turnData)
{
	(void)turnData;

	// Alias
	auto& player = *playerTurn.playerRef;

	double hpDelta = 0.0;

	// Decrement HP from player collisions
	for (const auto& collision : playerTurn.playerCollisions) {
		hpDelta -= TICK_INTERVAL * collision.opponentStrength;
	}

	// Increment HP from bonus effects
	hpDelta += playerTurn.effectAttributes.getAttributeChangeHp();

	// Decrement HP from "deflate"
	if (player.input->readInput().deflate) {
		double newDelta = hpDelta - DEFLATE_AMOUNT;
		if (!(player.hp + newDelta <= 0.0)) {
			// Can deflate, because it won't kill the player
			hpDelta = newDelta;
		}
	}

	// Don't grow if that would make you collide with an obstacle
	if ((hpDelta > 0.0) && m_stageObstacles->playerHasCollision(player.pos,
		getPlayerSize(player.hp + hpDelta)))
	{
		hpDelta = 0.0;
	}

	player.hp += hpDelta;

	if (player.hp <= 0.0) {
		// Player is dead

		// "Kill"
		m_players.erase(id);
		m_gsAgentProxy->killPlayer(id);

		// Create action
		auto res = std::make_shared<CoreActionRemovePlayer>(id);

		return res;
	} else {
		// Player is still alive

		// Create actions
		auto actionSetPlayerHp = std::make_shared<CoreActionSetPlayerHp>(id,
			player.hp);
		auto actionSetPlayerSize = std::make_shared<CoreActionSetPlayerSize>(id,
			getPlayerSize(player.hp));

		// Merge
		auto res = CoreActionMultiple::getMergedActions(actionSetPlayerHp,
			actionSetPlayerSize);
		
		return res;
	}
}

CoreActionPtr Core::applyPlayerBonusCollisions(PlayerId id,
	PlayerTurn& playerTurn, TurnData& turnData)
{
	(void)id;
	(void)turnData;

	// Alias
	auto& player = *playerTurn.playerRef;

	for (const auto& coll : playerTurn.bonusCollisions) {
		auto effect = m_stageBonuses->getBonusEffect(coll.id);
		player.bonusEffects.insert(effect);
	}

	return std::make_shared<CoreActionNone>();
}

Timer Core::createNewBonusTimer()
{
	static constexpr double PARAM_ALPHA = 10.95;
	static constexpr double PARAM_BETA = 0.95;
	static std::gamma_distribution distrib(PARAM_ALPHA, PARAM_BETA);
	
	double interval = distrib(getRNGine());
	return Timer(static_cast<std::clock_t>(interval * 1000.0));
}

void Core::resetBonusTimer()
{
	m_bonusTimer = createNewBonusTimer();
}

void Core::notifyAgents()
{
#ifdef INCLUDE_BENCHMARK
	Benchmark::get().beginMeasure(BENCH_ID_UPDATE_GSPROXY);
#endif // INCLUDE_BENCHMARK
	m_gsAgentProxy->update();
#ifdef INCLUDE_BENCHMARK
	Benchmark::get().endMeasure(BENCH_ID_UPDATE_GSPROXY);
#endif // INCLUDE_BENCHMARK

	for (auto& agent : m_aiAgents) {
		agent->plan();
	}
}

double Core::getPlayerSize(PlayerId id) const
{
	return getPlayerSize(m_players.at(id).hp);
}

double Core::getPlayerSize(double hp)
{
	constexpr double MIN_SIZE = 5.0;
	constexpr double BASE_SIZE = 50.0;
	// Should be `<= 1.0`, so players with more HP gain size more slowly and
	// the ones with less HP gain size faster
	constexpr double SIZE_POWER = 1.0;
	
	// Full HP -> 50.0
	// 0 HP -> 5.0
	// 2x Full HP -> 95.0 (if SIZE_POWER == 1.0)
	// 1/2x Full HP -> 27.5 (if SIZE_POWER == 1.0)
	return MIN_SIZE + std::pow(hp, SIZE_POWER) * (BASE_SIZE - MIN_SIZE);
}

double Core::getPlayerSpeed(PlayerId id) const
{
	return getPlayerSpeed(m_players.at(id).hp);
}

double Core::getPlayerSpeed(double hp)
{
	constexpr double MAX_SPEED = 1.0;
	constexpr double BASE_SPEED = 1.0 / 6.0;
	
	// Full HP -> 1/6 (BASE_SPEED)
	// 0 HP -> 1.0 (MAX_SPEED)
	// INF HP -> 0.0
	// This equation was a tough one...
	return (BASE_SPEED*MAX_SPEED) / ((MAX_SPEED - BASE_SPEED)*hp + BASE_SPEED);
}

double Core::getPlayerStrength(PlayerId id) const
{
	return getPlayerStrength(m_players.at(id).hp);
}

double Core::getPlayerStrength(double hp)
{
	static constexpr double STEP = 0.01;

	// Full HP -> 1/3400
	// 0 HP -> 0.0
	// 2x Full HP -> 1/1700
	// 1/2x Full HP -> 1/6800

	// Multiplication with of very low numbers may introduce strange behavior,
	// so instead, we define this as a step function and let the low numbers
	// round down to 0

	return roundfDownToMultiple(hp, STEP) * BASE_STRENGTH;
}

void Core::getPlayerMovementVector(PlayerId id, double& x, double& y) const
{
	const auto& player = m_players.at(id);
	return getPlayerMovementVector(player.input->readInput(),
		getPlayerSpeed(id), x, y);
}

void Core::getPlayerMovementVector(const PlayerInputFlags& inputFlags,
	double speed, double& x, double& y)
{
	inputFlags.toVector(x, y);
	x *= speed * TICK_INTERVAL;
	y *= speed * TICK_INTERVAL;
}

CoreActionPtr Core::initializeStage()
{
	// Initialize all
	auto actionInitPlayers = initializeStagePlayers();
	auto actionInitObstaclesAndBounds = initializeStageObstaclesAndBounds();
	auto actionInitBonuses = initializeStageBonuses();
	auto actionInitAiAgents = initializeStageAiAgents();

	m_isInitialized = true;
	
	// Merge
	auto res = CoreActionMultiple::getMergedActions(
		actionInitPlayers,
		actionInitObstaclesAndBounds,
		actionInitBonuses,
		actionInitAiAgents
	);
	
	return res;
}

CoreActionPtr Core::initializeStagePlayers()
{
	PlayerStateInternal newPlayer;

	CoreActionMultiple::ActionsCollection actionGroup;
	std::shared_ptr<CoreActionAddPlayer> actionAddPlayer;
	std::shared_ptr<CoreActionSetPlayerPos> actionSetPlayerPos;
	std::shared_ptr<CoreActionSetPlayerHp> actionSetPlayerHp;
	std::shared_ptr<CoreActionSetPlayerSize> actionSetPlayerSize;
	
	for (PlayerId id = 0; id < m_gsdata.players.size(); id++) {
		// Initialize player
		const auto& playerPos = m_gsdata.stage->getPlayers()[id];
		newPlayer.pos = Point_2(playerPos.x, playerPos.y);
		newPlayer.hp = PLAYER_HP_INITIAL;
		newPlayer.input = m_gsdata.players[id];
		m_players[id] = std::move(newPlayer);

		// Add to actions list
		actionAddPlayer = std::make_shared<CoreActionAddPlayer>(id);
		actionSetPlayerPos = std::make_shared<CoreActionSetPlayerPos>(id,
			fromCgalPoint(newPlayer.pos));
		actionSetPlayerHp = std::make_shared<CoreActionSetPlayerHp>(id,
			newPlayer.hp);
		actionSetPlayerSize = std::make_shared<CoreActionSetPlayerSize>(id,
			getPlayerSize(newPlayer.hp));
		actionGroup.push_back(actionAddPlayer);
		actionGroup.push_back(actionSetPlayerPos);
		actionGroup.push_back(actionSetPlayerHp);
		actionGroup.push_back(actionSetPlayerSize);
	}

	// Merge
	auto res = CoreActionMultiple::getMergedActions(actionGroup);

	return res;
}

CoreActionPtr Core::initializeStageObstaclesAndBounds()
{
	auto obstacles = getObstaclesList();
	auto bounds = getStageSize();

	CoreActionMultiple::ActionsCollection actionsGroup;
	std::shared_ptr<CoreActionAddObstacle> actionAddObstacle;
	std::shared_ptr<CoreActionSetStageSize> actionSetStageSize;

	m_stageObstacles = std::make_unique<StageObstacles>(obstacles, bounds);

	// Actions
	// Obstacles
	for (const auto& obstacle : obstacles) {
		actionAddObstacle = std::make_shared<CoreActionAddObstacle>(
			PolygonF(obstacle));
		actionsGroup.push_back(actionAddObstacle);
	}
	// Bounds
	actionSetStageSize = std::make_shared<CoreActionSetStageSize>(bounds);
	actionsGroup.push_back(actionSetStageSize);

	// Merge
	auto res = CoreActionMultiple::getMergedActions(actionsGroup);

	return res;
}

CoreActionPtr Core::initializeStageBonuses()
{
	m_stageBonuses = std::make_unique<StageBonuses>(
		getObstaclesList(), getStageSize());
	
	return std::make_shared<CoreActionNone>();
}

CoreActionPtr Core::initializeStageAiAgents()
{
	// Copy
	m_aiAgents = m_gsdata.aiAgents;

	// Create a game state proxy
	m_gsAgentProxy = std::make_shared<GameStateAgentProxyImplem>(*this);

	// Assign it to the agents
	for (auto& agent : m_aiAgents) {
		agent->assignProxy(m_gsAgentProxy);
	}
	
	return std::make_shared<CoreActionNone>();
}

CoreActionPtr Core::tick()
{
#ifdef INCLUDE_BENCHMARK
	Benchmark::get().beginMeasure(BENCH_ID_PL_ACTIONS);
#endif // INCLUDE_BENCHMARK
	auto res = playersActions();
#ifdef INCLUDE_BENCHMARK
	Benchmark::get().endMeasure(BENCH_ID_PL_ACTIONS);
#endif // INCLUDE_BENCHMARK
	notifyAgents();
	return res;
}

CoreActionPtr Core::playersActions()
{
	TurnData turnData;

	auto actionInitTurnData = initTurnData(turnData);
	auto actionApplyPlayerEffects = applyPlayerEffects(turnData);
	auto actionCalculateTrajectories = calculateTrajectories(turnData);
	auto actionFindPlayerAndBonusCollisions =
		findPlayerAndBonusCollisions(turnData);
	auto actionUpdatePlayersStates = updatePlayersStates(turnData);
	auto actionCheckIsGameOver = checkIsGameOver(turnData);
	auto actionClearBonuses = clearBonuses(turnData);
	auto actionGenerateBonus = generateBonus(turnData);

	auto res = CoreActionMultiple::getMergedActions(
		actionInitTurnData,
		actionApplyPlayerEffects,
		actionCalculateTrajectories,
		actionFindPlayerAndBonusCollisions,
		actionUpdatePlayersStates,
		actionCheckIsGameOver,
		actionClearBonuses,
		actionGenerateBonus
	);
	return res;
}

void Core::quit()
{
	for (auto& agent : m_aiAgents) {
		agent->kill();
	}
}

const CoreActionPtr Core::loopEvent()
{
	if (!m_isInitialized) {
		// This should happen only once -- `initializeStage()` changes the flag

		return initializeStage();
	} else {
		// Is initialized

		if (m_tickTimer.isLap()) {
			// Tick

			return tick();
		} else {
			// No tick

			return std::make_shared<CoreActionNone>();
		}
	}

	assert(m_isInitialized);
}

std::unordered_map<PlayerId,PlayerState> Core::getPlayerStates() const
{
	std::unordered_map<PlayerId,PlayerState> res;
	for (const auto& [id, state] : m_players) {
		res[id] = PlayerState{
			CGAL::to_double(state.pos.x()), // x
			CGAL::to_double(state.pos.y()), // y
			state.hp, // hp
			getPlayerSize(id), // size
		};
	}
	return res;
}

std::vector<StageObstacle> Core::getObstaclesList() const
{
	return m_gsdata.stage->getObstacles();
}

Size2d Core::getStageSize() const
{
	Size2d res(m_gsdata.stage->getWidth(), m_gsdata.stage->getHeight());
	return res;
}
