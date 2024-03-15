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
	, m_bonusTimer{createNewBonusTimer()}
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
			.bonusCollisions = std::vector<BonusCollision>(),
			.effectAttributes = EffectAttributes(),
		};
	}

	return std::make_shared<CoreActionNone>();
}

std::shared_ptr<CoreAction> Core::applyPlayerEffects(TurnData& turnData)
{
	for (auto& [id, turn] : turnData.playerTurns) {
		auto& player = m_players[id];

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
			if (idA != idB) { // A player cannot collide with itself
				// Minimum distance between players' trajectories
				double minSqdist = playerTurnA.trajectory.minSqdist(
					playerTurnB.trajectory);
				// Square of sum of the players' sizes
				double playerSqsizes = sqr(getPlayerSize(idA)
					+ getPlayerSize(idB));
				if (minSqdist <= playerSqsizes) {
					// Add collision with B to A's player collisions
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

std::shared_ptr<CoreAction> Core::findBonusCollisions(TurnData& turnData)
{
	const auto& bonuses = m_stageBonuses->getBonuses();

	for (auto& [playerId, playerTurn] : turnData.playerTurns) {
		for (const auto& [bonusId, bonusData] : bonuses) {
			// Create zero length trajectory representing the movement of the
			// bonus (it doesn't move, but we need the trajectory).
			Trajectory bonusTraj(toCgalPoint(bonusData.position));
			// Minimum distance between the player's and bonus's trajectories
			double minSqdist = playerTurn.trajectory.minSqdist(bonusTraj);
			// Square of sum of the player's and bonus's size
			double sqSizes = sqr(getPlayerSize(playerId) + BONUS_RADIUS);
			if (minSqdist <= sqSizes) {
				// Add the collision to the player's bonus collisions
				BonusCollision collision = {.id = bonusId};
				playerTurn.bonusCollisions.push_back(std::move(collision));
				// Add the collision to turn data
				turnData.collectedBonuses.insert(bonusId);
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

		// Move player
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

		double hpDelta = 0.0;

		// Decrement HP from player collisions
		for (const auto& collision : playerTurn.playerCollisions) {
			hpDelta -= TICK_INTERVAL * collision.opponentStrength;
		}

		// Increment HP from bonus effects
		hpDelta += playerTurn.effectAttributes.getAttributeChangeHp();

		// Don't grow if that would make you collide with an obstacle
		if (m_stageObstacles->playerHasCollision(player.pos, getPlayerSize(
			player.hp + hpDelta)))
		{
			hpDelta = 0.0;
		}

		player.hp += hpDelta;

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

std::shared_ptr<CoreAction> Core::applyBonusCollisions(TurnData& turnData)
{
	for (const auto& [id, turn] : turnData.playerTurns) {
		auto& player = m_players[id];

		for (const auto& coll : turn.bonusCollisions) {
			auto effect = m_stageBonuses->getBonusEffect(coll.id);
			player.bonusEffects.insert(effect);
		}
	}

	return std::make_shared<CoreActionNone>();
}

std::shared_ptr<CoreAction> Core::clearBonuses(TurnData& turnData)
{
	std::vector<std::shared_ptr<CoreAction>> actionsGroup;
	std::shared_ptr<CoreActionRemoveBonus> actionRemoveBonus;

	for (BonusId id : turnData.collectedBonuses) {
		// Clear
		m_stageBonuses->clearBonus(id);

		// Reset timer
		resetBonusTimer();

		// Add to actions list
		actionRemoveBonus = std::make_shared<CoreActionRemoveBonus>(id);
		actionsGroup.push_back(std::move(actionRemoveBonus));
	}

	// Merge
	auto res = std::make_shared<CoreActionMultiple>(actionsGroup);

	return res;
}

std::shared_ptr<CoreAction> Core::generateBonus(TurnData& turnData)
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
			const PointF& bonusPos = m_stageBonuses->getBonuses().at(bonusId).position;

			auto res = std::make_shared<CoreActionAddBonus>(bonusId, bonusPos);
			return res;
		}
	}
	
	// Else
	return std::make_shared<CoreActionNone>();
}

Timer Core::createNewBonusTimer()
{
	static constexpr double PARAM_ALPHA = 11.625;
	static constexpr double PARAM_BETA = 0.6;
	static std::gamma_distribution distrib(PARAM_ALPHA, PARAM_BETA);
	
	double interval = distrib(getRNGine());
	return Timer(static_cast<std::clock_t>(interval * 1000.0));
}

void Core::resetBonusTimer()
{
	m_bonusTimer = createNewBonusTimer();
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

	auto actionInitTurnData = initTurnData(turnData);
	auto actionApplyPlayerEffects = applyPlayerEffects(turnData);
	auto actionCalculateTrajectories = calculateTrajectories(turnData);
	auto actionFindPlayerCollisions = findPlayerCollisions(turnData);
	auto actionFindBonusCollisions = findBonusCollisions(turnData);
	auto actionMovePlayers = movePlayers(turnData);
	auto actionChangePlayersHp = changePlayersHp(turnData);
	auto actionApplyBonusCollisions = applyBonusCollisions(turnData);
	auto actionClearBonuses = clearBonuses(turnData);
	auto actionGenerateBonus = generateBonus(turnData);

	auto res = std::make_shared<CoreActionMultiple>(
		actionInitTurnData,
		actionApplyPlayerEffects,
		actionCalculateTrajectories,
		actionFindPlayerCollisions,
		actionFindBonusCollisions,
		actionMovePlayers,
		actionChangePlayersHp,
		actionApplyBonusCollisions,
		actionClearBonuses,
		actionGenerateBonus
	);
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
