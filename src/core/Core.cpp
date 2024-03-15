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

void Core::inputToVector(const PlayerInputFlags & input, double & x, double & y)
{
	// sin(pi/4)
	// Position delta per axis for diagonal movement (same for X and Y axis)
	constexpr double DIAG_MOVEMENT_PERAXIS_LENGTH =
		0.70710678118654752440084436210485;

	// If none or both keys for horizontal movement are pressed, the player
	// won't move horizontally
	bool isHorzMove = (input.left != input.right);
	// Same as isHorzMove
	bool isVertMove = (input.up != input.down);

	if (!isHorzMove && !isVertMove) {
		// Doesn't move

		x = 0.0;
		y = 0.0;
	} else if (isHorzMove && isVertMove) {
		// Diagonal

		x = (input.left ? -DIAG_MOVEMENT_PERAXIS_LENGTH
			: DIAG_MOVEMENT_PERAXIS_LENGTH);
		y = (input.up ? -DIAG_MOVEMENT_PERAXIS_LENGTH
			: DIAG_MOVEMENT_PERAXIS_LENGTH);
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

Core::Core(const std::shared_ptr<IStageSerializer> stage,
	const std::vector<std::shared_ptr<IPlayerInput>>& players)
	: m_isInitialized{false}
	, m_stageInitializer{stage}
	, m_playersInitializer{players}
	, m_tickTimer(TICK_INTERVAL)
	, m_bonusTimer(1000)
{
	assert(players.size() <= stage->getPlayers().size());
}

std::shared_ptr<CoreAction> Core::initTurnData(TurnData& turnData)
{
	turnData.playerTurns.reserve(m_players.size());
	
	for (const auto& [id, state] : m_players) {
		(void)state;

		turnData.playerTurns[id] = PlayerTurn{
#ifndef OLD_TRAJECTORY_ALGORITHM
			.trajectory = Trajectory(state.pos),
#else
			.trajectory = Trajectory(),
#endif
			.playerCollisions = std::vector<PlayerCollision>(),
		};
	}

	return std::make_shared<CoreActionNone>();
}

std::shared_ptr<CoreAction> Core::calculateTrajectories(TurnData& turnData)
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

std::shared_ptr<CoreAction> Core::findPlayerCollisions(TurnData& turnData)
{
	for (auto& [idA, playerTurnA] : turnData.playerTurns) {
		for (auto& [idB, playerTurnB] : turnData.playerTurns) {
			if (idA != idB) {
				double minSqdist = playerTurnA.trajectory.minSqdist(
					playerTurnB.trajectory);
				double playerSqsizes = sqr(getPlayerSize(idA)
					+ getPlayerSize(idB));
				if (minSqdist <= playerSqsizes) {
					PlayerCollision collision = {
						.opponentStrength = getPlayerStrength(idB)
					};
					playerTurnA.playerCollisions.push_back(collision);
				}
			}
		}
	}

	return std::make_shared<CoreActionNone>();
}

std::shared_ptr<CoreAction> Core::movePlayers(TurnData& turnData)
{
	CoreActionMultiple::ActionsCollection actionsGroup;
	std::shared_ptr<CoreActionSetPlayerPos> actionSetPlayerPos;

	for (auto& [id, playerTurn] : turnData.playerTurns) {
		auto& player = m_players[id];

#ifndef OLD_TRAJECTORY_ALGORITHM
		player.pos = playerTurn.trajectory.end();
#else
		player.pos = playerTurn.trajectory.last().getPEnd();
#endif

		// Add to actions list
		actionSetPlayerPos = std::make_shared<CoreActionSetPlayerPos>(id,
			fromCgalPoint(player.pos));
		actionsGroup.push_back(actionSetPlayerPos);
	}

	auto res = std::make_shared<CoreActionMultiple>(actionsGroup);
	return res;
}

std::shared_ptr<CoreAction> Core::changePlayersHp(TurnData& turnData)
{
	CoreActionMultiple::ActionsCollection actionSetPlayerHpGroup;
	std::shared_ptr<CoreActionSetPlayerHp> actionSetPlayerHp;
	CoreActionMultiple::ActionsCollection actionSetPlayerSizeGroup;
	std::shared_ptr<CoreActionSetPlayerSize> actionSetPlayerSize;

	for (auto& [id, playerTurn] : turnData.playerTurns) {
		auto& player = m_players[id];

		for (const auto& collision : playerTurn.playerCollisions) {
			player.hp -= TICK_INTERVAL * collision.opponentStrength;
		}

		// Add to actions list
		actionSetPlayerHp = std::make_shared<CoreActionSetPlayerHp>(id,
			player.hp * PLAYER_HP_FACTOR);
		actionSetPlayerSize = std::make_shared<CoreActionSetPlayerSize>(id,
			getPlayerSize(player.hp));
		actionSetPlayerHpGroup.push_back(actionSetPlayerHp);
		actionSetPlayerSizeGroup.push_back(actionSetPlayerSize);
	}

	// Merge
	auto actionsGroup1 = std::make_shared<CoreActionMultiple>(
		actionSetPlayerHpGroup);
	auto actionsGroup2 = std::make_shared<CoreActionMultiple>(
		actionSetPlayerSizeGroup);
	auto res = std::make_shared<CoreActionMultiple>(actionsGroup1,
		actionsGroup2);
	
	return res;
}

std::shared_ptr<CoreAction> Core::generateBonus(TurnData& turnData)
{
	(void)turnData;

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
		const PointF& bonusPos = m_stageBonuses->getBonuses().at(bonusId);

		auto res = std::make_shared<CoreActionAddBonus>(bonusId, bonusPos);
		return res;
	} else {
		return std::make_shared<CoreActionNone>();
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
	constexpr double BASE_STRENGTH = 1.0/3400.0;

	// Full HP -> 1/3400
	// 0 HP -> 0.0
	// 2x Full HP -> 1/1700
	// 1/2x Full HP -> 1/6800
	return hp * BASE_STRENGTH;
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
	inputToVector(inputFlags, x, y);
	x *= speed * TICK_INTERVAL;
	y *= speed * TICK_INTERVAL;
}

std::shared_ptr<CoreAction> Core::initializeStage()
{
	PlayerStateInternal newPlayer;

	CoreActionMultiple::ActionsCollection actionPlayerGroup;
	std::shared_ptr<CoreActionAddPlayer> actionAddPlayer;
	std::shared_ptr<CoreActionSetPlayerPos> actionSetPlayerPos;
	std::shared_ptr<CoreActionSetPlayerHp> actionSetPlayerHp;
	std::shared_ptr<CoreActionSetPlayerSize> actionSetPlayerSize;
	CoreActionMultiple::ActionsCollection actionObstacleGroup;
	std::shared_ptr<CoreActionAddObstacle> actionAddObstacle;
	std::shared_ptr<CoreActionSetStageSize> actionSetStageSize;

	// Players
	for (PlayerId id = 0; id < m_playersInitializer.size(); id++) {
		// Initialize player
		const auto& playerPos = m_stageInitializer->getPlayers()[id];
		newPlayer.pos = Point_2(playerPos.x, playerPos.y);
		newPlayer.hp = PLAYER_HP_INITIAL;
		newPlayer.input = m_playersInitializer[id];
		m_players[id] = std::move(newPlayer);

		// Add to actions list
		actionAddPlayer = std::make_shared<CoreActionAddPlayer>(id);
		actionSetPlayerPos = std::make_shared<CoreActionSetPlayerPos>(id,
			fromCgalPoint(newPlayer.pos));
		actionSetPlayerHp = std::make_shared<CoreActionSetPlayerHp>(id,
			newPlayer.hp * PLAYER_HP_FACTOR);
		actionSetPlayerSize = std::make_shared<CoreActionSetPlayerSize>(id,
			getPlayerSize(newPlayer.hp));
		actionPlayerGroup.push_back(actionAddPlayer);
		actionPlayerGroup.push_back(actionSetPlayerPos);
		actionPlayerGroup.push_back(actionSetPlayerHp);
		actionPlayerGroup.push_back(actionSetPlayerSize);
	}

	// Obstacles and bounds
	Size2d bounds = getStageSize();;
	m_stageObstacles = std::make_unique<StageObstacles>(
		getObstaclesList(), bounds);
	
	// Add to actions list (obstacles)
	auto obstacles = m_stageInitializer->getObstacles();
	for (const auto& obstacle : obstacles) {
		actionAddObstacle = std::make_shared<CoreActionAddObstacle>(
			PolygonF(obstacle));
		actionObstacleGroup.push_back(actionAddObstacle);
	}

	m_stageBonuses = std::make_unique<StageBonuses>(
		getObstaclesList(), getStageSize());

	m_isInitialized = true;

	// Add to actions list (bounds)
	actionSetStageSize = std::make_shared<CoreActionSetStageSize>(bounds);
	
	// Merge
	auto actionsGroup1 = std::make_shared<CoreActionMultiple>(
		actionPlayerGroup);
	auto actionsGroup2 = std::make_shared<CoreActionMultiple>(
		actionObstacleGroup);
	auto res = std::make_shared<CoreActionMultiple>(actionsGroup1,
		actionsGroup2, actionSetStageSize);
	
	return res;
}

std::shared_ptr<CoreAction> Core::tick()
{
	return playersActions();
}

std::shared_ptr<CoreAction> Core::playersActions()
{
	TurnData turnData;

	auto action1 = initTurnData(turnData);
	auto action2 = calculateTrajectories(turnData);
	auto action3 = findPlayerCollisions(turnData);
	auto action4 = movePlayers(turnData);
	auto action5 = changePlayersHp(turnData);
	auto action6 = generateBonus(turnData);

	auto res = std::make_shared<CoreActionMultiple>(action1, action2, action3,
		action4, action5, action6);
	return res;
}

const std::shared_ptr<CoreAction> Core::loopEvent()
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
			.x = CGAL::to_double(state.pos.x()),
			.y = CGAL::to_double(state.pos.y()),
			.hp = state.hp * PLAYER_HP_FACTOR,
			.size = getPlayerSize(id),
		};
	}
	return res;
}

std::vector<StageObstacle> Core::getObstaclesList() const
{
	return m_stageInitializer->getObstacles();
}

Size2d Core::getStageSize() const
{
	Size2d res(m_stageInitializer->getWidth(), m_stageInitializer->getHeight());
	return res;
}
