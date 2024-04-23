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

//#define DO_LOG_BFS

#include "aiplayeragent/BFSPredatorAIPlayerAgent.hpp"

#ifdef DO_LOG_BFS
#include <chrono>
#include <fstream>
#include <iostream>
#include <stack>
#include <unordered_set>

static std::ofstream logFile("bfs.log");

class ExecTimeMeasure {
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_t0;
public:
	ExecTimeMeasure()
		: m_t0{std::chrono::high_resolution_clock::now()}
	{}
	void output() {
		auto t1 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> ms = t1 - m_t0;
		logFile << "Time taken: " << ms.count() << " ms" << std::endl;
	}
};
#endif // DO_LOG_BFS

PlayerInputFlags BFSPredatorAIPlayerAgent::chooseNextAction(
	const GameStateAgentProxy::PlayerState* victim)
{
#ifdef DO_LOG_BFS
	ExecTimeMeasure tmes;
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

#ifdef DO_LOG_BFS
	logFile << "BREADTH FIRST SEARCH" << std::endl;
		int generated = 1;
		int explored = 1;
#endif // DO_LOG_BFS

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
				generated++;
#endif // DO_LOG_BFS

				// The cell must be accessible by the player.
				if (bfsSucc->cell.getNearestObstacleDistance() > mySqsize) {
					if (bfsSucc->cell == sGoal) {
						// Found path

#ifdef DO_LOG_BFS
						logFile << "  Generated: " << generated << std::endl;
						logFile << "  Explored: " << explored << std::endl;
						logFile << "  ----------------" << std::endl;
						tmes.output();
#endif // DO_LOG_BFS

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
#ifdef DO_LOG_BFS
						explored++;
#endif // DO_LOG_BFS

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
