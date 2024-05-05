/**
 * @file BFSPredatorAIPlayerAgent.cpp
 * @author Tomáš Ludrovan
 * @brief BFSPredatorAIPlayerAgent class
 * @version 0.1
 * @date 2024-04-09
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifdef INCLUDE_BENCHMARK
#define DO_LOG_BFS
#endif // INCLUDE_BENCHMARK

#include "aiplayeragent/BFSPredatorAIPlayerAgent.hpp"

#ifdef DO_LOG_BFS
#include "utilities/benchmark/Benchmark.hpp"

static constexpr const char* BENCH_ID = "agent-bfs";
#endif // DO_LOG_BFS

PlayerInputFlags BFSPredatorAIPlayerAgent::chooseNextAction(
	const GameStateAgentProxy::PlayerState* victim)
{
#ifdef DO_LOG_BFS
	AutoLogger_NodesSqdist sqdistLogger(BENCH_ID, CGAL::squared_distance(
		getMyState().pos, victim->pos));
	AutoLogger_Measure measureLogger(BENCH_ID);
#endif // DO_LOG_BFS

	// Quick access

	const auto& me = getMyState();
	const auto& grid = gsProxy->getStageGridModel();
	auto mySqsize = sqr(me.size);

	// Initial state
	auto sStart = grid.getCellAt(me.pos);
	// Goal state
	auto sGoal = grid.getCellAt(victim->pos);

	// Already in goal?
	if (sStart == sGoal) return PlayerInputFlags();

	// Possible actions
	// The actions are limited because diagonal movement is more likely to not
	// work correctly.
	std::array<Direction8, 4> bfsActions{DIR8_N, DIR8_E, DIR8_S, DIR8_W};

	BFSOpen bfsOpen;
	BFSClosed bfsClosed;

	// Initial node
	auto idsStart = std::make_shared<BFSNode>(BFSNode{
		sStart,             // cell
		PlayerInputFlags(), // action
		nullptr             // prev
	});
	bfsOpen.push(idsStart);
#ifdef INCLUDE_BENCHMARK
	sqdistLogger.incNodeCount();
#endif // INCLUDE_BENCHMARK

	// When OPEN is empty, it is unsuccessful search.
	while (!bfsOpen.empty()) {
		// `currNode = bfsOpen.pop()`
		auto currNode = bfsOpen.front();
		bfsOpen.pop();
		// ... and add the popped node to CLOSED
		bfsClosed.insert(currNode);

		// Expand
		for (auto bfsAction : bfsActions) {
			if (currNode->cell.hasNeighbor(bfsAction)) {
				// The action is legal in this state.

				auto bfsSucc = std::make_shared<BFSNode>(BFSNode{
					currNode->cell.getNeighbor(bfsAction), // cell
					bfsAction,                             // action
					currNode.get()                         // prev
				});
#ifdef DO_LOG_BFS
				sqdistLogger.incNodeCount();
#endif // DO_LOG_BFS

				// The cell must be accessible by the player.
				if (bfsSucc->cell.getNearestObstacleDistance() > mySqsize) {
					if (bfsSucc->cell == sGoal) {
						// Found path

						auto goalPtr = bfsSucc.get();

						// Find the node at the depth just below the root.
						while (goalPtr->prev->cell != sStart) {
							goalPtr = goalPtr->prev;
						}

						return goalPtr->action;
					}

					if (!bfsOpen.contains(bfsSucc)
						&& bfsClosed.find(bfsSucc) == bfsClosed.end())
					{
						// Brand new state

						bfsOpen.push(bfsSucc);
					}
				}
			}
		}
	}

	return PlayerInputFlags();
}

void BFSPredatorAIPlayerAgent::doPlan()
{
	auto victim = chooseVictim();

	if (victim == nullptr) {
		// No victim; can't plan

		m_input = PlayerInputFlags();
	} else {
		m_input = chooseNextAction(victim);
	}
}

BFSPredatorAIPlayerAgent::BFSPredatorAIPlayerAgent(PlayerId playerId)
	: AIPlayerAgentBase(playerId)
	, PredatorAIPlayerAgentBase(playerId)
{}
