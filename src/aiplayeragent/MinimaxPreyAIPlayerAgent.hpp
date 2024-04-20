/**
 * @file MinimaxPreyAIPlayerAgent.hpp
 * @author Tomáš Ludrovan
 * @brief MinimaxPreyAIPlayerAgent class
 * @version 0.1
 * @date 2024-04-17
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef MINIMAXPREYAIPLAYERAGENT_HPP
#define MINIMAXPREYAIPLAYERAGENT_HPP

// Versions:
//  - version 1
//    - Simple Minimax
//  - version 2
//    - Alpha-beta pruning
#define MINIMAX_PREY_VERSION 2

#include <array>
#include <limits>
#include <memory>
#include <stack>

#include "types.hpp"
#include "playerinput/PlayerInputFlags.hpp"
#include "aiplayeragent/PreyAIPlayerAgentBase.hpp"
#include "aiplayeragent/StageGridModel.hpp"

class MinimaxPreyAIPlayerAgent : public PreyAIPlayerAgentBase {
private:
	// Node evaluation value type
	typedef double NodeEval;
	typedef Direction8 MinimaxAction;
	typedef std::array<MinimaxAction, 5> MinimaxActions;
	/**
	 * @brief Node in the Minimax search tree.
	 */
	struct MinimaxNode {
		// Cell corresponding to the position of the "Max" player
		StageGridModel::Cell maxCell;
		// Cell corresponding to the position of the "Min" player
		StageGridModel::Cell minCell;
		// Index of the action leading to the best leaf node
		int bestActionIdx;
		// Index of the action to perform next from this node
		int nextActionIdx;
		// Depth of the node within the search tree
		int depth;
#if MINIMAX_PREY_VERSION == 1
		// Evaluation of the node
		NodeEval eval;
#else // MINIMAX_PREY_VERSION != 1
		// Alpha parameter of the node
		NodeEval alpha;
		// Beta parameter of the node
		NodeEval beta;
#endif // MINIMAX_PREY_VERSION != 1
	};
	/**
	 * @brief Node in the Minimax search tree.
	 */
	typedef std::shared_ptr<MinimaxNode> MinimaxNodeP;
	/**
	 * @brief Stack for the Minimax nodes.
	 */
	class MinimaxStack : public std::stack<MinimaxNodeP, std::vector<MinimaxNodeP>> {
	public:
		void reserve(size_t n) {
			c.reserve(n);
		}
	};
private:
	// Possible actions
	// The actions are limited to cardinal only, because diagonal movement
	// is more likely to not work correctly.
	static constexpr MinimaxActions MINIMAX_ACTIONS{DIR8_NONE, DIR8_E, DIR8_S,
		DIR8_W, DIR8_N};
	// Maximum depth of the search tree
	static constexpr int MAX_DEPTH =
#if MINIMAX_PREY_VERSION == 1
		6
#else // MINIMAX_PREY_VERSION != 1
		10
#endif // MINIMAX_PREY_VERSION != 1
	;

	static constexpr NodeEval EVAL_LO = std::numeric_limits<NodeEval>::min();
	static constexpr NodeEval EVAL_HI = std::numeric_limits<NodeEval>::max();

	PlayerInputFlags m_input;

	/**
	 * @brief Chooses which action to take next.
	 * 
	 * @note Performs Minimax search.
	 * 
	 * @param attacker The opponent which poses the biggest threat.
	 */
	PlayerInputFlags chooseNextAction(
		const GameStateAgentProxy::PlayerState* attacker);
	/**
	 * @brief Creates successor of the `n` node in the `direction` direction.
	 * 
	 * @param n Node to generate successor for.
	 * @param direction The direction of the successor from the `n` node.
	 */
	static MinimaxPreyAIPlayerAgent::MinimaxNodeP getSucc(
		const MinimaxPreyAIPlayerAgent::MinimaxNodeP& n, Direction8 direction);
	/**
	 * @brief Checks if successor can be generated.
	 * 
	 * @param n Node to generate successor for.
	 * @param direction The direction of the successor from the `n` node.
	 * @param maxSqsize Square of the radius of Max's bubble.
	 * @param minSqsize Square of the radius of Min's bubble.
	 */
	static bool hasSucc(const MinimaxPreyAIPlayerAgent::MinimaxNodeP& n,
		Direction8 direction, double maxSqsize, double minSqsize);
	/**
	 * @brief Checks whether it is Max's turn in the node `n`.
	 */
	static bool isMaxTurn(const MinimaxPreyAIPlayerAgent::MinimaxNodeP& n);
	/**
	 * @brief Evaluates a leaf node.
	 * 
	 * @param maxCell Max cell of the leaf node.
	 * @param minCell Min cell of the leaf node.
	 */
	static MinimaxPreyAIPlayerAgent::NodeEval evalLeaf(
		const StageGridModel::Cell& maxCell,
		const StageGridModel::Cell& minCell);
	/**
	 * @brief Calculates the taxicab distance between two nodes.
	 */
	static MinimaxPreyAIPlayerAgent::NodeEval getTaxicab(
		const StageGridModel::Cell& c1, const StageGridModel::Cell& c2);
	/**
	 * @brief After Minimax search chooses the input which would be best made
	 *        at the current state.
	 * 
	 * @param initialNode Current state of the game represented as MinimaxNode.
	 * @param me Player state which belongs to this agent.
	 */
	PlayerInputFlags minimaxGetBestInput(
		const MinimaxPreyAIPlayerAgent::MinimaxNodeP& initialNode,
		const GameStateAgentProxy::PlayerState& me) const;
protected:
	void doPlan() override;
	PlayerInputFlags doGetPlayerInput() override { return m_input; }
public:
	MinimaxPreyAIPlayerAgent(PlayerId playerId);
};

#endif // MINIMAXPREYAIPLAYERAGENT_HPP
