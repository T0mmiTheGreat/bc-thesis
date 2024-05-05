/**
 * @file AstarPredatorAIPlayerAgent.cpp
 * @author Tomáš Ludrovan
 * @brief AstarPredatorAIPlayerAgent class
 * @version 0.1
 * @date 2024-04-09
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "aiplayeragent/AstarPredatorAIPlayerAgent.hpp"

PlayerInputFlags AstarPredatorAIPlayerAgent::chooseNextAction(
	const GameStateAgentProxy::PlayerState* victim)
{
#ifdef DO_LOG_ASTAR_PREDATOR
	AutoLogger_NodesSqdist sqdistLogger(BENCH_ID, CGAL::squared_distance(
		getMyState().pos, victim->pos));
	AutoLogger_Measure measureLogger(BENCH_ID);
#endif // DO_LOG_ASTAR_PREDATOR

	auto d = astarDataCreate(victim);
	astarDataInit(d);

#ifdef DO_LOG_ASTAR_PREDATOR
	d.sqdistLogger = &sqdistLogger;
	d.measureLogger = &measureLogger;
	sqdistLogger.incNodeCount(); // Initial node
#endif // DO_LOG_ASTAR_PREDATOR

	// Already in goal?
	if (d.sStart == d.sGoal) return PlayerInputFlags();

#if ASTAR_PREDATOR_VERSION == 3
	while (!d.isFinished) {
		astarProcessNextNode(d);
	}
#else // ASTAR_PREDATOR_VERSION != 3
	while (!d.astarOpen.empty()) {
		astarProcessNextNode(d);
		if (d.isFinished) {
			break;
		}
	}
#endif // ASTAR_PREDATOR_VERSION != 3

	return d.bestInput;
}

AstarPredatorAIPlayerAgent::AstarData AstarPredatorAIPlayerAgent::astarDataCreate(
	const GameStateAgentProxy::PlayerState* victim) const
{
	const auto& me = getMyState();
	const auto& grid = gsProxy->getStageGridModel();
	auto sStart = grid.getCellAt(me.pos);
	auto sGoal = grid.getCellAt(victim->pos);

	AstarData res = {
		me,                 // me
		victim,             // victim
		grid,               // grid
		sqr(me.size),       // mySqsize

		sStart,             // sStart
		sGoal,              // sGoal

		AstarOpen(),        // astarOpen
		AstarClosed(),      // astarClosed

		1,                  // generatedNodes

		false,              // isFinished
		PlayerInputFlags(), // bestInput

#if ASTAR_PREDATOR_VERSION == 3
		nullptr,            // nearestNode
#endif // ASTAR_PREDATOR_VERSION == 3
#ifdef DO_LOG_ASTAR_PREDATOR
		nullptr,            // sqdistLogger
		nullptr,            // measureLogger
#endif // DO_LOG_ASTAR_PREDATOR
	};
	return res;
}

void AstarPredatorAIPlayerAgent::astarDataInit(AstarData& d) const
{
	// Space is less of a problem than performance
	d.astarOpen.reserve(d.MAX_GENERATED_NODES);
	d.astarClosed.reserve(d.MAX_GENERATED_NODES);

	auto initialHvalue = getHvalue(d.sStart, d.sGoal);
	// Initial node
	auto astarStart = std::make_shared<AstarNode>(AstarNode{
		d.sStart,      // cell
		DIR8_NONE,     // direction
		0,             // gvalue
		initialHvalue, // hvalue
	});
	d.astarOpen.push(astarStart);
}

void AstarPredatorAIPlayerAgent::astarProcessNextNode(AstarData& d) const
{
#if ASTAR_PREDATOR_VERSION == 3
	astarRefreshNearestNode(d);
#endif // ASTAR_PREDATOR_VERSION == 3

	if (isAstarFinished(d)) {
		d.isFinished = true;
		d.bestInput = astarGetBestInput(d);
	} else {
		// Not finished; move the node from OPEN to CLOSED and expand it
		
		// The "next" node
		// MUST NOT be reference; the value will be "popped", invalidating the
		// reference.
		auto n = d.astarOpen.top();
		d.astarOpen.pop(); // Pops `n`
		d.astarClosed.insert(n);

		astarExpandNode(d, n);
	}
}

void AstarPredatorAIPlayerAgent::astarExpandNode(AstarData& d,
	const AstarNodeP& n) const
{
	// Try all actions
	for (auto astarAction : ASTAR_ACTIONS) {
		if (n->cell.hasNeighbor(astarAction)) {
			// The action is legal in this state

			auto astarSucc = getSucc(n, astarAction, d.sGoal);
			d.generatedNodes++;

#ifdef DO_LOG_ASTAR_PREDATOR
			d.sqdistLogger->incNodeCount();
#endif // DO_LOG_ASTAR_PREDATOR

			if (astarSucc->cell.getNearestObstacleDistance() > d.mySqsize) {
				// The node can be reached by the agent's bubble

				if (!d.astarOpen.contains(astarSucc)
					&& !d.astarClosed.contains(astarSucc))
				{
					// Brand new node

					// Insert it to OPEN
					d.astarOpen.push(astarSucc);
				}
			}

			if (d.generatedNodes >= d.MAX_GENERATED_NODES) {
				break;
			}
		}
	}
}

bool AstarPredatorAIPlayerAgent::isAstarFinished(AstarData& d) const
{
	// Node at the top of the OPEN
	const auto& n = d.astarOpen.top();
	return (d.generatedNodes >= d.MAX_GENERATED_NODES) || (n->cell == d.sGoal)
#if ASTAR_PREDATOR_VERSION == 3
		|| (d.astarOpen.empty())
#endif // ASTAR_PREDATOR_VERSION == 3
	;
}

PlayerInputFlags AstarPredatorAIPlayerAgent::astarGetBestInput(
	AstarData& d) const
{
#if ASTAR_PREDATOR_VERSION == 1
	return PlayerInputFlags(d.astarOpen.top()->direction);
#elif ASTAR_PREDATOR_VERSION >= 2
#	if ASTAR_PREDATOR_VERSION == 2
	// Node at the top of the OPEN
	const auto& n = d.astarOpen.top();
#	elif ASTAR_PREDATOR_VERSION == 3
	const auto& n = d.nearestNode;
#	endif // ASTAR_PREDATOR_VERSION == 3
	// The direction taken in the root node to reach `n`
	auto dir = n->direction;
	// Neighbor of the root cell in the `dir` direction
	auto sStartNeigh = d.sStart.getNeighbor(dir);
	// Position of the neighbor (i.e., the cell's center)
	auto destPos = sStartNeigh.getPosition();

	auto bestInput = PlayerInputFlags();
	auto bestSqdist = std::numeric_limits<double>::infinity();

	// Try out all inputs and see which one brings the bubble closest to the
	// neighbor node
	for (auto input : generateInputs()) {
		auto pos = gsProxy->calculateNewPlayerPos(d.me.pos, input, d.me);
		auto sqdist = CGAL::squared_distance(pos, destPos);
		if (sqdist < bestSqdist) {
			bestInput = input;
			bestSqdist = sqdist;
		}
	}
	return bestInput;
#endif // ASTAR_PREDATOR_VERSION >= 2
}

#if ASTAR_PREDATOR_VERSION == 3
void AstarPredatorAIPlayerAgent::astarRefreshNearestNode(AstarData& d) const
{
	if (d.astarOpen.empty()) return;

	const auto& n = d.astarOpen.top();
	if (d.nearestNode == nullptr || n->hvalue < d.nearestNode->hvalue) {
		d.nearestNode = n;
	}
}
#endif // ASTAR_PREDATOR_VERSION == 3

AstarPredatorAIPlayerAgent::NodeEval AstarPredatorAIPlayerAgent::getHvalue(
	const StageGridModel::Cell& s, const StageGridModel::Cell& sGoal)
{
	// Manhattan distance -- given we can only move in cardinal directions, the
	// shortest distance between two locations is given by the sum of the
	// absolute differences of their respective Cartesian coordinates.
	return std::abs(sGoal.getPosition().x() - s.getPosition().x())
		+  std::abs(sGoal.getPosition().y() - s.getPosition().y());
}

AstarPredatorAIPlayerAgent::AstarNodeP AstarPredatorAIPlayerAgent::getSucc(
	const AstarPredatorAIPlayerAgent::AstarNodeP& n, Direction8 direction,
	const StageGridModel::Cell& sGoal)
{
	// Successor state
	auto s = n->cell.getNeighbor(direction);
	// Successor's `direction` property -- if the current node's `direction` is
	// "NONE", then the current node is the root node, so the successor's
	// `direction` is given by its actual direction from the current node (which
	// is the root). Otherwise, it just "inherits" the value from the current
	// node (which is not the root).
	auto nDirection = (n->direction != DIR8_NONE ? n->direction : direction);
	// Successor's g() value
	NodeEval nGvalue = n->gvalue + 1;
	// Successor's h() value
	NodeEval nHvalue = getHvalue(s, sGoal);

	auto res = std::make_shared<AstarNode>(AstarNode{s, nDirection, nGvalue,
		nHvalue});
	return res;
}

void AstarPredatorAIPlayerAgent::doPlan()
{
	auto victim = chooseVictim();

	if (victim == nullptr) {
		// No victim; can't plan

		m_input = PlayerInputFlags();
	} else {
		m_input = chooseNextAction(victim);
	}
}

AstarPredatorAIPlayerAgent::AstarPredatorAIPlayerAgent(PlayerId playerId)
	: AIPlayerAgentBase(playerId)
	, PredatorAIPlayerAgentBase(playerId)
{}
