/**
 * @file MinimaxPreyAIPlayerAgent.cpp
 * @author Tomáš Ludrovan
 * @brief MinimaxPreyAIPlayerAgent class
 * @version 0.1
 * @date 2024-04-17
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "aiplayeragent/MinimaxPreyAIPlayerAgent.hpp"

PlayerInputFlags MinimaxPreyAIPlayerAgent::chooseNextAction(
	const GameStateAgentProxy::PlayerState* attacker)
{
	// Quick access

	const auto& me = getMyState();
	const auto& grid = gsProxy->getStageGridModel();
	auto mySqsize = sqr(me.size);
	auto attSqsize = sqr(attacker->size);

	// Cell which belongs to this agent
	auto myCell = grid.getCellAt(me.pos);
	// Cell which belongs to the attacker
	auto attCell = grid.getCellAt(attacker->pos);

	MinimaxStack nodeStack;
	nodeStack.reserve(static_cast<size_t>(MAX_DEPTH));

	MinimaxNodeP initialNode = std::make_shared<MinimaxNode>(MinimaxNode{
		myCell,    // maxCell
		attCell,   // minCell
		0,         // bestActionIdx
		0,         // nextActionIdx
		1,         // depth
		EVAL_LO,   // eval
	});
	nodeStack.push(initialNode);

	while (!nodeStack.empty()) {
		auto& top = nodeStack.top();
		if (top->depth == MAX_DEPTH
			|| top->nextActionIdx == static_cast<int>(MINIMAX_ACTIONS.size()))
		{
			auto pop = top;
			nodeStack.pop();
			if (!nodeStack.empty()) {
				auto& newTop = nodeStack.top();
				if (isMaxTurn(newTop)) {
					// Max's turn
					if (newTop->eval < pop->eval) {
						newTop->eval = pop->eval;
						newTop->bestActionIdx = newTop->nextActionIdx - 1;
					}
				} else {
					// Min's turn
					if (newTop->eval > pop->eval) {
						newTop->eval = pop->eval;
						newTop->bestActionIdx = newTop->nextActionIdx - 1;
					}
				}	
			}
		} else {
			auto actn = MINIMAX_ACTIONS[top->nextActionIdx];
			top->nextActionIdx++;
			if (hasSucc(top, actn, mySqsize, attSqsize)) {
				auto succ = getSucc(top, actn);
				nodeStack.push(succ);
			}
		}
	}

	return minimaxGetBestInput(initialNode, me);
}

MinimaxPreyAIPlayerAgent::MinimaxNodeP MinimaxPreyAIPlayerAgent::getSucc(
	const MinimaxPreyAIPlayerAgent::MinimaxNodeP& n, Direction8 direction)
{
	auto maxCell = (isMaxTurn(n)
		? n->maxCell.getNeighbor(direction)
		: n->maxCell);
	auto minCell = (isMaxTurn(n)
		? n->minCell
		: n->minCell.getNeighbor(direction));
	int depth = n->depth + 1;
	NodeEval eval = (depth == MAX_DEPTH
		? getTaxicab(maxCell, minCell)
		: (isMaxTurn(n) ? EVAL_HI : EVAL_LO));
	
	MinimaxNodeP res = std::make_shared<MinimaxNode>(MinimaxNode{
		maxCell,   // maxCell
		minCell,   // minCell
		0,         // bestActionIdx
		0,         // nextActionIdx
		depth,     // depth
		eval,      // eval
	});
	return res;
}

bool MinimaxPreyAIPlayerAgent::hasSucc(
	const MinimaxPreyAIPlayerAgent::MinimaxNodeP& n, Direction8 direction,
	double maxSqsize, double minSqsize)
{
	if (isMaxTurn(n)) {
		if (!n->maxCell.hasNeighbor(direction)) return false;
		auto neigh = n->maxCell.getNeighbor(direction);
		return (neigh.getNearestObstacleDistance() >= maxSqsize);
	} else {
		if (!n->minCell.hasNeighbor(direction)) return false;
		auto neigh = n->minCell.getNeighbor(direction);
		return (neigh.getNearestObstacleDistance() >= minSqsize);
	}
}

bool MinimaxPreyAIPlayerAgent::isMaxTurn(
	const MinimaxPreyAIPlayerAgent::MinimaxNodeP& n)
{
	return (n->depth % 2 == 1);
}

MinimaxPreyAIPlayerAgent::NodeEval MinimaxPreyAIPlayerAgent::getTaxicab(
	const StageGridModel::Cell& c1,const StageGridModel::Cell& c2)
{
	return abs(c1.getPosition().x() - c2.getPosition().x())
		+  abs(c1.getPosition().y() - c2.getPosition().y());
}

PlayerInputFlags MinimaxPreyAIPlayerAgent::minimaxGetBestInput(
	const MinimaxPreyAIPlayerAgent::MinimaxNodeP& initialNode,
	const GameStateAgentProxy::PlayerState& me) const
{
	// The direction taken in the root node to reach `n`
	auto dir = MINIMAX_ACTIONS[initialNode->bestActionIdx];
	// Neighbor of the root cell in the `dir` direction
	auto sStartNeigh = initialNode->maxCell.getNeighbor(dir);
	// Position of the neighbor (i.e., the cell's center)
	auto destPos = sStartNeigh.getPosition();

	auto bestInput = PlayerInputFlags();
	auto bestSqdist = std::numeric_limits<double>::infinity();

	// Try out all inputs and see which one brings the bubble closest to the
	// neighbor node
	for (auto input : generateInputs()) {
		auto pos = gsProxy->calculateNewPlayerPos(me.pos, input, me);
		auto sqdist = CGAL::squared_distance(pos, destPos);
		if (sqdist < bestSqdist) {
			bestInput = input;
			bestSqdist = sqdist;
		}
	}
	return bestInput;
}

void MinimaxPreyAIPlayerAgent::doPlan()
{
	auto attacker = chooseAttacker();

	if (attacker == nullptr) {
		// No victim; can't plan

		m_input = PlayerInputFlags();
	} else {
		m_input = chooseNextAction(attacker);
	}
}

MinimaxPreyAIPlayerAgent::MinimaxPreyAIPlayerAgent(PlayerId playerId)
	: AIPlayerAgentBase(playerId)
	, PreyAIPlayerAgentBase(playerId)
{}
