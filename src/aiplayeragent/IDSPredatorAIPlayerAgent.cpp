/**
 * @file IDSPredatorAIPlayerAgent.cpp
 * @author Tomáš Ludrovan
 * @brief IDSPredatorAIPlayerAgent class
 * @version 0.1
 * @date 2024-04-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifdef INCLUDE_BENCHMARK
#define DO_LOG_IDS
#endif // INCLUDE_BENCHMARK


//#define USE_CLOSED_LIST

#include "aiplayeragent/IDSPredatorAIPlayerAgent.hpp"

#ifdef DO_LOG_IDS
#include "utilities/benchmark/Benchmark.hpp"

static constexpr const char* BENCH_ID = "agent-ids";
#endif // DO_LOG_IDS

PlayerInputFlags IDSPredatorAIPlayerAgent::chooseNextAction(
	const GameStateAgentProxy::PlayerState* victim)
{
#ifdef DO_LOG_IDS
	AutoLogger_NodesSqdist sqdistLogger(BENCH_ID, CGAL::squared_distance(
		getMyState().pos, victim->pos));
	AutoLogger_Measure measureLogger(BENCH_ID);
#endif // DO_LOG_IDS

	// Quick access

	const auto& me = getMyState();
	const auto& grid = gsProxy->getStageGridModel();

	// Initial state
	auto sStart = grid.getCellAt(me.pos);
	// Goal state
	auto sGoal = grid.getCellAt(victim->pos);

	// Already in goal?
	if (sStart == sGoal) return PlayerInputFlags();

	// Possible actions
	// The actions are limited because diagonal movement is more likely to not
	// work correctly.
	std::array<Direction8, 4> idsActions{DIR8_N, DIR8_E, DIR8_S, DIR8_W};

	IDSOpen idsOpen;
#ifdef USE_CLOSED_LIST
	IDSClosed idsClosed;
#endif // USE_CLOSED_LIST

	// Initial node
	auto idsStart = std::make_shared<IDSNode>(IDSNode{
		sStart,             // cell
		0,                  // depth
		0,                  // actionIdx
	});
	// Do not add the start node to the OPEN yet!
#ifdef DO_LOG_IDS
	sqdistLogger.incNodeCount(); // Is not added, but is generated
#endif // DO_LOG_IDS

	bool reachedMaxDepth = true;
	// Once the max depth has not been reached the search stops (unsuccessful).
	for (int maxDepth = 1; reachedMaxDepth; ++maxDepth) {
		reachedMaxDepth = false;
		idsStart->actionIdx = 0;
		idsOpen.push(idsStart);
#ifdef USE_CLOSED_LIST
		idsClosed.clear();
#endif // USE_CLOSED_LIST

		while (!idsOpen.empty()) {
			auto currNode = idsOpen.top();

			if (currNode->actionIdx == idsActions.size()) {
				// All actions have been tried
#ifdef USE_CLOSED_LIST
				idsClosed.insert(idsOpen.top());
#endif // USE_CLOSED_LIST
				idsOpen.pop();
			} else {
				auto action = idsActions[currNode->actionIdx];
				++currNode->actionIdx;

				if (currNode->cell.hasNeighbor(action)) {
					// Action can be applied.

					auto idsSucc = std::make_shared<IDSNode>(IDSNode{
						currNode->cell.getNeighbor(action), // cell
						currNode->depth + 1,                // depth
						0,                                  // actionIdx
					});
#ifdef DO_LOG_IDS
					sqdistLogger.incNodeCount();
#endif // DO_LOG_IDS

					if (idsSucc->cell.getNearestObstacleDistance() > sqr(me.size))
					if (!idsOpen.contains(idsSucc)
#ifdef USE_CLOSED_LIST
						&& !(idsClosed.find(idsSucc) != idsClosed.end())
#endif // USE_CLOSED_LIST
					) {
						// The node is not a duplicate of another one.

						if (idsSucc->cell == sGoal) {
							// Found the path

							// The optimal action is the last action which has
							// been taken in the root.
							auto optimalAction =
								idsActions[idsStart->actionIdx - 1];
							return PlayerInputFlags(optimalAction);
						} else if (idsSucc->depth == maxDepth) {
							// The node lies in the maximum depth

							reachedMaxDepth = true;
							// No need to add the node to the OPEN; it has no
							// successors, and we've already checked it is not
							// a goal node.
						} else {
							// Not a goal, not in the max depth

							idsOpen.push(idsSucc);
						}
					}
				}
			}
		}
	}

	return PlayerInputFlags();
}

void IDSPredatorAIPlayerAgent::doPlan()
{
	auto victim = chooseVictim();

	if (victim == nullptr) {
		// No victim; can't plan

		m_input = PlayerInputFlags();
	} else {
		m_input = chooseNextAction(victim);
	}
}

IDSPredatorAIPlayerAgent::IDSPredatorAIPlayerAgent(PlayerId playerId)
	: AIPlayerAgentBase(playerId)
	, PredatorAIPlayerAgentBase(playerId)
{}
