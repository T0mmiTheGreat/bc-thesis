/**
 * @file WallAwareBFSPredatorAIPlayerAgent.cpp
 * @author Tomáš Ludrovan
 * @brief WallAwareBFSPredatorAIPlayerAgent class
 * @version 0.1
 * @date 2024-04-06
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "aiplayeragent/WallAwareBFSPredatorAIPlayerAgent.hpp"

#ifdef INCLUDE_BENCHMARK
#include "utilities/benchmark/Benchmark.hpp"

static constexpr const char* BENCH_ID = "agent-wall-aware-bfs";
#endif // INCLUDE_BENCHMARK

PlayerInputFlags WallAwareBFSPredatorAIPlayerAgent::chooseNextAction(
	const GameStateAgentProxy::PlayerState* victim)
{
#ifdef INCLUDE_BENCHMARK
	AutoLogger_Measure measureLogger(BENCH_ID);
	AutoLogger_NodesSqdist sqdistLogger(BENCH_ID, CGAL::squared_distance(
		getMyState().pos, victim->pos));
#endif // INCLUDE_BENCHMARK

	const auto& me = getMyState();

	if (CGAL::squared_distance(me.pos, victim->pos) <= sqr(me.size + victim->size)) {
		// Already touching; no need to move
		return PlayerInputFlags();
	}

	BFSOpen bfsOpen;
	BFSClosed bfsClosed;
	const auto bfsActions = generateInputs();

	// Initial (root) node
	BFSNodeP bfsInitial = std::make_shared<BFSNode>(BFSNode{
		me.pos,             // pos
		PlayerInputFlags(), // action
		nullptr             // prev
	});
	bfsOpen.push(bfsInitial);
#ifdef INCLUDE_BENCHMARK
	sqdistLogger.incNodeCount();
#endif // INCLUDE_BENCHMARK

	bool isSolutionFound = false;
	BFSNodeP goalNodePtr = nullptr;

	while (!bfsOpen.empty()) {
		// Pop the front node from OPEN
		auto currNode = bfsOpen.front();
		bfsOpen.pop();
		// Insert it to CLOSED
		bfsClosed.insert(currNode);

		// Expand
		for (const auto& bfsAction : bfsActions) {
			auto succ = bfsCreateSucc(currNode, bfsAction, me);
#ifdef INCLUDE_BENCHMARK
			sqdistLogger.incNodeCount();
#endif // INCLUDE_BENCHMARK

			if (bfsGoalTest(succ, me, victim)) {
				// Found the goal node

				isSolutionFound = true;
				goalNodePtr = succ;
				break;
			}

			if (!bfsNodeExists(succ, bfsOpen, bfsClosed)) {
				// The node is netiher in OPEN nor CLOSED -- push it to OPEN

				bfsOpen.push(succ);
			}
		}

		if (isSolutionFound) {
			// Found the goal node

			BFSNode* currNodePtr = goalNodePtr.get();
			// Trace the path to one of the immediate successors of the root
			while (!BFSNodeEqual()(currNodePtr->prev, bfsInitial)) {
				currNodePtr = currNodePtr->prev;
			}

			// Return the action required to get to the successor
			return currNodePtr->action;
		}
	}

	// No solution (cannot reach the victim)
	return PlayerInputFlags();
}

bool WallAwareBFSPredatorAIPlayerAgent::bfsGoalTest(
	const WallAwareBFSPredatorAIPlayerAgent::BFSNodeP& n,
	const GameStateAgentProxy::PlayerState& me,
	const GameStateAgentProxy::PlayerState* victim)
{
	// My trajectory
	Trajectory t1(Segment_2(n->prev->pos, n->pos));
	// My victim's trajectory
	Trajectory t2(victim->pos);

	// Minimum squared distance between me and my victim
	double minSqdist = t1.minSqdist(t2);
	// Square of the radii of mine and my victim's
	double playerSqSizes = sqr(me.size + victim->size);

	return (minSqdist <= playerSqSizes);
}

WallAwareBFSPredatorAIPlayerAgent::BFSNodeP
WallAwareBFSPredatorAIPlayerAgent::bfsCreateSucc(
	WallAwareBFSPredatorAIPlayerAgent::BFSNodeP& n,
	const PlayerInputFlags& action, const GameStateAgentProxy::PlayerState& me)
{
	BFSNodeP res = std::make_shared<BFSNode>(BFSNode{
		gsProxy->calculateNewPlayerPos(n->pos, action, me), // pos
		action,                                             // action
		n.get()                                             // prev
	});

	return res;
}

bool WallAwareBFSPredatorAIPlayerAgent::bfsNodeExists(
	WallAwareBFSPredatorAIPlayerAgent::BFSNodeP& n,
	const WallAwareBFSPredatorAIPlayerAgent::BFSOpen& bfsOpen,
	const WallAwareBFSPredatorAIPlayerAgent::BFSClosed& bfsClosed)
{
	return (bfsClosed.find(n) != bfsClosed.end()) // Not present in CLOSED
		|| (bfsOpen.find(n) != bfsOpen.end());    // Not present in OPEN
}

void WallAwareBFSPredatorAIPlayerAgent::doPlan()
{
	auto victim = chooseVictim();

	if (victim == nullptr) {
		// No victim; can't plan

		m_input = PlayerInputFlags();
	} else {
		m_input = chooseNextAction(victim);
	}
}

WallAwareBFSPredatorAIPlayerAgent::WallAwareBFSPredatorAIPlayerAgent(PlayerId playerId)
	: AIPlayerAgentBase(playerId)
	, PredatorAIPlayerAgentBase(playerId)
{}
