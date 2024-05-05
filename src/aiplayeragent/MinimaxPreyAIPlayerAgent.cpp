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

#ifdef INCLUDE_BENCHMARK
#define DO_LOG_MINIMAX_PREY
#endif // INCLUDE_BENCHMARK

#ifdef DO_LOG_MINIMAX_PREY
#include "utilities/benchmark/Benchmark.hpp"

static constexpr const char* BENCH_ID = "agent-minimax-prey";
#endif // DO_LOG_MINIMAX_PREY

PlayerInputFlags MinimaxPreyAIPlayerAgent::chooseNextAction(
	const GameStateAgentProxy::PlayerState* attacker)
{
#ifdef DO_LOG_MINIMAX_PREY
	AutoLogger_Measure measureLogger(BENCH_ID);
#endif // DO_LOG_MINIMAX_PREY

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
#if MINIMAX_PREY_VERSION == 1
		EVAL_LO,   // eval
#else // MINIMAX_PREY_VERSION != 1
		EVAL_LO,   // alpha
		EVAL_HI,   // beta
#endif // MINIMAX_PREY_VERSION != 1
	});
	nodeStack.push(initialNode);

	// While there are any unexplored nodes within the tree
	while (!nodeStack.empty()) {
		auto& top = nodeStack.top();
		// Reached max depth, or tried all actions, or (MINIMAX_PREY_VERSION
		// > 1) alpha >= beta
		if ((top->depth == MAX_DEPTH)
			|| (top->nextActionIdx == static_cast<int>(MINIMAX_ACTIONS.size()))
#if MINIMAX_PREY_VERSION > 1
			|| (top->alpha >= top->beta)
#endif // MINIMAX_PREY_VERSION > 1
		) {
			auto pop = top;
			nodeStack.pop();
			if (!nodeStack.empty()) {
				auto& newTop = nodeStack.top();
				if (isMaxTurn(newTop)) {
					// Max's turn in `newTop`, Min's turn in `pop`
#if MINIMAX_PREY_VERSION == 1
					if (newTop->eval < pop->eval) {
						// Found better node evaluation

						newTop->eval = pop->eval;
						newTop->bestActionIdx = newTop->nextActionIdx - 1;
					}
#else // MINIMAX_PREY_VERSION != 1
					// `beta` is the node evaluation of `pop` (Min) and `alpha`
					// is the node evaluation of `newTop` (Max)
					if (pop->beta > newTop->alpha) {
						// Found better node evaluation

						newTop->alpha = pop->beta;
						newTop->bestActionIdx = newTop->nextActionIdx - 1;
					}
#endif // MINIMAX_PREY_VERSION != 1
				} else {
					// Min's turn in `newPop`, Max's turn in `pop`
#if MINIMAX_PREY_VERSION == 1
					if (newTop->eval > pop->eval) {
						// Found worse node evaluation

						newTop->eval = pop->eval;
						newTop->bestActionIdx = newTop->nextActionIdx - 1;
					}
#else // MINIMAX_PREY_VERSION != 1
					// `alpha` is the node evaluation of `pop` (Max) and `beta`
					// is the node evaluation of `newTop` (Min)
					if (pop->alpha < newTop->beta) {
						// Found worse node evaluation

						newTop->beta = pop->alpha;
						newTop->bestActionIdx = newTop->nextActionIdx - 1;
					}
#endif // MINIMAX_PREY_VERSION != 1
				}	
			}
		} else {
			auto actn = MINIMAX_ACTIONS[top->nextActionIdx];
			top->nextActionIdx++;
			if (hasSucc(top, actn, mySqsize, attSqsize)) {
				// The cell does have a neighbor in the `actn` direction

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
		? n->maxCell.getNeighbor(direction) // Max will move
		: n->maxCell);                      // Max won't move
	auto minCell = (isMaxTurn(n)
		? n->minCell                          // Min won't move
		: n->minCell.getNeighbor(direction)); // Min will move
	int depth = n->depth + 1;
#if MINIMAX_PREY_VERSION == 1
	NodeEval eval = (depth == MAX_DEPTH
		// It's the leaf node -- calculate its evaluation.
		? evalLeaf(maxCell, minCell)
		// Not the leaf node -- initialize as the worst possible evaluation for
		// the player whose turn it is.
		: (isMaxTurn(n) ? EVAL_HI : EVAL_LO));
#else // MINIMAX_PREY_VERSION != 1
	NodeEval alpha = n->alpha;
	NodeEval beta = n->beta;
	if (depth == MAX_DEPTH) {
		alpha = beta = evalLeaf(maxCell, minCell);
	}
#endif // MINIMAX_PREY_VERSION != 1
	
	MinimaxNodeP res = std::make_shared<MinimaxNode>(MinimaxNode{
		maxCell,   // maxCell
		minCell,   // minCell
		0,         // bestActionIdx
		0,         // nextActionIdx
		depth,     // depth
#if MINIMAX_PREY_VERSION == 1
		eval,      // eval
#else // MINIMAX_PREY_VERSION != 1
		alpha,     // alpha
		beta,     // beta
#endif // MINIMAX_PREY_VERSION != 1
	});
	return res;
}

bool MinimaxPreyAIPlayerAgent::hasSucc(
	const MinimaxPreyAIPlayerAgent::MinimaxNodeP& n, Direction8 direction,
	double maxSqsize, double minSqsize)
{
	if (isMaxTurn(n)) {
		// Is there a path?
		if (!n->maxCell.hasNeighbor(direction)) return false;

		auto neigh = n->maxCell.getNeighbor(direction);
		// Do I fit through?
		return (neigh.getNearestObstacleDistance() >= maxSqsize);
	} else { // Min's turn
		// Is there a path?
		if (!n->minCell.hasNeighbor(direction)) return false;

		auto neigh = n->minCell.getNeighbor(direction);
		// Do I fit through?
		return (neigh.getNearestObstacleDistance() >= minSqsize);
	}
}

bool MinimaxPreyAIPlayerAgent::isMaxTurn(
	const MinimaxPreyAIPlayerAgent::MinimaxNodeP& n)
{
	// Odd turns are Max's, even turns are Min's (root is 1 -- Max).
	return (n->depth % 2 == 1);
}

MinimaxPreyAIPlayerAgent::NodeEval MinimaxPreyAIPlayerAgent::evalLeaf(
	const StageGridModel::Cell& maxCell,
	const StageGridModel::Cell& minCell)
{
	return getTaxicab(maxCell, minCell);
}

MinimaxPreyAIPlayerAgent::NodeEval MinimaxPreyAIPlayerAgent::getTaxicab(
	const StageGridModel::Cell& c1,const StageGridModel::Cell& c2)
{
	// Taxicab distance:
	//   0 1 2 3 4
	// 0 XX======++
	// 1         ||
	// 2         ()
	// distance = 6

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
