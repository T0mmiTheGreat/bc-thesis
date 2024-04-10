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
	std::array<Direction8, 4> astarActions{DIR8_N, DIR8_E, DIR8_S, DIR8_W};

	AstarOpen astarOpen;
	AstarClosed astarClosed;

	// Initial node
	auto idsStart = std::make_shared<AstarNode>(AstarNode{
		sStart,                   // cell
		DIR8_NONE,                // direction
		getHvalue(sStart, sGoal), // fvalue
		0                         // gvalue
	});
	astarOpen.push(idsStart);

	static constexpr int MAX_GENERATED_NODES = 2200;
	int generatedNodes = 1;

	while (!astarOpen.empty()) {
		auto currNode = astarOpen.top();

		if ((generatedNodes >= MAX_GENERATED_NODES)
			|| currNode->cell == sGoal)
		{
#if ASTAR_PREDATOR_VERSION == 1
			return PlayerInputFlags(currNode->direction);
#elif ASTAR_PREDATOR_VERSION == 2
			auto initialDir = currNode->direction;
			auto destPos = sStart.getNeighbor(initialDir).getPosition();

			auto bestInput = PlayerInputFlags();
			auto bestSqdist = std::numeric_limits<double>::infinity();

			for (auto input : generateInputs()) {
				auto posAtt = gsProxy->calculateNewPlayerPos(me.pos, input, me);
				auto sqdist = CGAL::squared_distance(posAtt, destPos);
				if (sqdist < bestSqdist) {
					bestInput = input;
					bestSqdist = sqdist;
				}
			}
			return bestInput;
#endif // ASTAR_PREDATOR_VERSION == 2
		}

		astarOpen.pop();
		astarClosed.insert(currNode);

		for (auto astarAction : astarActions) {
			if (currNode->cell.hasNeighbor(astarAction)) {
				auto astarSucc = getSucc(currNode, astarAction, sGoal);
				generatedNodes++;

				// The cell must be accessible by the player.
				if (astarSucc->cell.getNearestObstacleDistance() > mySqsize) {
					if (!astarOpen.contains(astarSucc)
						&& !astarClosed.contains(astarSucc))
					{
						astarOpen.push(astarSucc);
					}
				}

				if (generatedNodes >= MAX_GENERATED_NODES) {
					break;
				}
			}
		}
	}

	return PlayerInputFlags();
}

AstarPredatorAIPlayerAgent::NodeEval AstarPredatorAIPlayerAgent::getHvalue(
	const StageGridModel::Cell& s, const StageGridModel::Cell& sGoal)
{
	return std::abs(sGoal.getPosition().x() - s.getPosition().x())
		+  std::abs(sGoal.getPosition().y() - s.getPosition().y());
}

AstarPredatorAIPlayerAgent::AstarNodeP AstarPredatorAIPlayerAgent::getSucc(
	const AstarPredatorAIPlayerAgent::AstarNodeP& n, Direction8 direction,
	const StageGridModel::Cell& sGoal)
{
	auto s = n->cell.getNeighbor(direction);
	auto nDirection = (n->direction != DIR8_NONE ? n->direction : direction);
	NodeEval nGvalue = n->gvalue + 1;
	NodeEval nHvalue = getHvalue(s, sGoal);
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
