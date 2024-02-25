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
	: m_tickTimer(TICK_INTERVAL)
{
	initializeStage(stage, players);
}

void Core::initTurnData(TurnData& turnData)
{
	turnData.playerTurns.reserve(m_players.size());
	
	for (const auto& [id, state] : m_players) {
		(void)state;

		turnData.playerTurns[id] = PlayerTurn{
			.trajectory = Trajectory(),
			.playerCollisions = std::unordered_set<PlayerId>(),
		};
	}
}

void Core::calculateTrajectories(TurnData& turnData)
{
	double vx, vy;
	Point_2 source;
	Vector_2 v;
	
	for (auto& [id, playerTurn] : turnData.playerTurns) {

		getPlayerMovementVector(id, vx, vy);
		source = m_players[id].pos;
		v = Vector_2(vx, vy);

		playerTurn.trajectory.segment.lineSegment.seg = Segment_2(source,
			source + v);
		m_stageObstacles->adjustPlayerTrajectory(playerTurn.trajectory,
			getPlayerSize(id));
	}
}

void Core::findPlayerCollisions(TurnData& turnData)
{
	for (auto& [idA, playerTurnA] : turnData.playerTurns) {
		for (auto& [idB, playerTurnB] : turnData.playerTurns) {
			if (idA != idB) {
				double minSqdist = playerTurnA.trajectory.minSqdist(
					playerTurnB.trajectory);
				double playerSqsizes = sqr(getPlayerSize(idA)
					+ getPlayerSize(idB));
				if (minSqdist <= playerSqsizes) {
					playerTurnA.playerCollisions.insert(idB);
				}
			}
		}
	}
}

void Core::movePlayers(TurnData& turnData)
{
	for (auto& [id, playerTurn] : turnData.playerTurns) {
		auto& player = m_players[id];

		player.pos = playerTurn.trajectory.segment.lineSegment.seg.target();
	}
}

void Core::changePlayersHp(TurnData& turnData)
{
	for (auto& [id, playerTurn] : turnData.playerTurns) {
		auto& player = m_players[id];

		for (const auto& otherId : playerTurn.playerCollisions) {
			player.hp -= TICK_INTERVAL * getPlayerStrength(otherId);
		}
	}
}

double Core::getPlayerSize(PlayerId id) const
{
	return getPlayerSize(m_players.at(id).hp);
}

double Core::getPlayerSize(double hp)
{
	constexpr double BASE_SIZE = 50.0;
	// Should be `<= 1.0`, so players with more HP gain size more slowly and
	// the ones with less HP gain size faster
	constexpr double SIZE_POWER = 1.0;
	
	// Full HP -> 100.0
	// 0 HP -> 0.0
	// 2x Full HP -> 200.0 (if SIZE_POWER == 1.0)
	// 1/2x Full HP -> 50.0 (if SIZE_POWER == 1.0)
	return std::pow(hp, SIZE_POWER) * BASE_SIZE;
}

double Core::getPlayerSpeed(PlayerId id) const
{
	return getPlayerSpeed(m_players.at(id).hp);
}

double Core::getPlayerSpeed(double hp)
{
	constexpr double BASE_SPEED = 1.0 / 6.0;
	
	// Full HP -> 0.1667
	// 0 HP -> 0.0
	// 2x Full HP -> 0.0833
	// 1/2x Full HP -> 0.3333
	return BASE_SPEED / hp;
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

void Core::initializeStage(const std::shared_ptr<IStageSerializer> stage,
	const std::vector<std::shared_ptr<IPlayerInput>>& players)
{
	assert(players.size() <= stage->getPlayers().size());

	PlayerStateInternal newPlayer;

	// Players
	for (size_t i = 0; i < players.size(); i++) {
		const auto& playerPos = stage->getPlayers()[i];
		newPlayer.pos = Point_2(playerPos.x, playerPos.y);
		newPlayer.hp = PLAYER_HP_INITIAL;
		newPlayer.input = players[i];
		m_players[i] = newPlayer;
	}

	// Obstacles
	Size2d bounds(stage->getWidth(), stage->getHeight());
	m_stageObstacles = std::make_unique<StageObstacles>(stage->getObstacles(),
		bounds);
}

void Core::tick()
{
	playersActions();
}

void Core::playersActions()
{
	TurnData turnData;

	initTurnData(turnData);
	calculateTrajectories(turnData);
	findPlayerCollisions(turnData);
	movePlayers(turnData);
	changePlayersHp(turnData);
}

void Core::loopEvent()
{
	if (m_tickTimer.isLap()) {
		tick();
	}
}

std::unordered_map<PlayerId,PlayerState> Core::getPlayerStates() const
{
	std::unordered_map<PlayerId,PlayerState> res;
	for (const auto& [id, state] : m_players) {
		res[id] = PlayerState{
			.x = CGAL::to_double(state.pos.x()),
			.y = CGAL::to_double(state.pos.y()),
			.hp = state.hp,
			.size = getPlayerSize(id),
		};
	}
	return res;
}

std::vector<StageObstacle> Core::getObstaclesList() const
{
	return m_stageObstacles->getObstaclesList();
}

Size2d Core::getStageSize() const
{
	return m_stageObstacles->getStageSize();
}
