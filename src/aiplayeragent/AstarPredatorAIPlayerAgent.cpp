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
	auto d = astarDataCreate(victim);
	astarDataInit(d);

	// Already in goal?
	if (d.sStart == d.sGoal) return PlayerInputFlags();

	while (!d.astarOpen.empty()) {
		astarProcessNextNode(d);
		if (d.isFinished) {
			break;
		}
	}

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
		me,                // me
		victim,            // victim
		grid,              // grid
		sqr(me.size),      // mySqsize

		sStart,            // sStart
		sGoal,             // sGoal

		AstarOpen(),       // astarOpen
		AstarClosed(),     // astarClosed

		1,                 // generatedNodes

		false,             // isFinished
		PlayerInputFlags() // bestInput
	};
	return res;
}

void AstarPredatorAIPlayerAgent::astarDataInit(AstarData& d) const
{
	// Space is less of a problem than performance
	d.astarOpen.reserve(d.MAX_GENERATED_NODES);
	d.astarClosed.reserve(d.MAX_GENERATED_NODES);

	// Initial node
	auto astarStart = std::make_shared<AstarNode>(AstarNode{
		d.sStart,                     // cell
		DIR8_NONE,                    // direction
		getHvalue(d.sStart, d.sGoal), // fvalue
		0                             // gvalue
	});
	d.astarOpen.push(astarStart);
}

void AstarPredatorAIPlayerAgent::astarProcessNextNode(AstarData& d) const
{
	if (isAstarFinished(d)) {
		d.isFinished = true;
		d.bestInput = astarGetBestInput(d);
	} else {
		// Not finished; move the node from OPEN to CLOSED and expand it

		// `n = d.astarOpen.pop()`
		// MUST NOT be reference; the value will be "popped", invalidating the
		// reference.
		auto n = d.astarOpen.top();
		d.astarOpen.pop();
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

			if (astarSucc->cell.getNearestObstacleDistance() > d.mySqsize) {
				// The node can be reached by the agent's bubble

				if (!d.astarOpen.contains(astarSucc)
					&& !d.astarClosed.contains(astarSucc))
				{
					// Brand new node

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
	return (d.generatedNodes >= d.MAX_GENERATED_NODES) || (n->cell == d.sGoal);
}

PlayerInputFlags AstarPredatorAIPlayerAgent::astarGetBestInput(
	AstarData& d) const
{
#if ASTAR_PREDATOR_VERSION == 1
	return PlayerInputFlags(currNode->direction);
#elif ASTAR_PREDATOR_VERSION == 2
	// Node at the top of the OPEN
	const auto& n = d.astarOpen.top();
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
#endif // ASTAR_PREDATOR_VERSION == 2
}

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
	// Successor's f() value
	NodeEval nFvalue = nGvalue + nHvalue;

	auto res = std::make_shared<AstarNode>(AstarNode{s, nDirection, nGvalue,
		nFvalue});
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
