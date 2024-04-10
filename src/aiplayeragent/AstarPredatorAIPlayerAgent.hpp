/**
 * @file AstarPredatorAIPlayerAgent.hpp
 * @author Tomáš Ludrovan
 * @brief AstarPredatorAIPlayerAgent class
 * @version 0.1
 * @date 2024-04-09
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef ASTARPREDATORAIPLAYERAGENT_HPP
#define ASTARPREDATORAIPLAYERAGENT_HPP

// Versions:
//  - version 1
//    - Chooses the direction which was taken in the root to reach the goal
//      node.
//    - Once the maximum number of nodes is generated, the node at the top of
//      the OPEN is selected as the goal.
//    - If OPEN becomes empty, doesn't move at all.
//  - version 2
//    - Checks which neighbor of the root lies on the path to the goal, then
//      chooses the action which gets him closest to that neighbor.
//    - Once the maximum number of nodes is generated, the node at the top of
//      the OPEN is selected as the goal.
//    - If OPEN becomes empty, doesn't move at all.
//  - version 3
//    - Once the maximum number of nodes is generated, the node closest to the
//      goal is selected as the goal.
#define ASTAR_PREDATOR_VERSION 3

#include <memory>
#include <queue>
#include <unordered_set>
#include <vector>

#include "playerinput/PlayerInputFlags.hpp"
#include "aiplayeragent/PredatorAIPlayerAgentBase.hpp"
#include "aiplayeragent/StageGridModel.hpp"

class AstarPredatorAIPlayerAgent : public PredatorAIPlayerAgentBase {
private:
	typedef double NodeEval;
	typedef std::array<Direction8, 4> AstarActions;
	/**
	 * @brief Node in the A* search tree.
	 */
	struct AstarNode {
		// Cell corresponding to the node
		StageGridModel::Cell cell;
		// The direction taken in the root node
		Direction8 direction;
		// g(s)
		NodeEval gvalue;
		// h(s)
		NodeEval hvalue;

		NodeEval fvalue() const { return gvalue + hvalue; }
	};
	/**
	 * @brief Node in the A* search tree.
	 */
	typedef std::shared_ptr<AstarNode> AstarNodeP;
	/**
	 * @brief Hash function object for the A* node.
	 */
	struct AstarNodeHash {
		size_t operator()(const AstarNodeP& key) const {
			return StageGridModel::Cell::Hash()(key->cell);
		}
	};
	/**
	 * @brief Equality comparison function object for the A* node.
	 */
	struct AstarNodeEqual {
		bool operator()(const AstarNodeP& lhs, const AstarNodeP& rhs) const {
			return (lhs->cell.getPosition() == rhs->cell.getPosition());
		}
	};
	/**
	 * @brief Priority comparison function object for the A* node.
	 */
	struct AstarNodeCompare {
		bool operator()(const AstarNodeP& lhs, const AstarNodeP& rhs) const {
			return (lhs->fvalue() > rhs->fvalue());
		}
	};
	/**
	 * @brief Priority queue for A* nodes.
	 */
	class AstarPrioq
		: public std::priority_queue<AstarNodeP, std::vector<AstarNodeP>,
		  AstarNodeCompare>
	{
	public:
		/**
		 * @brief Increases the capacity of the container to a value greater
		 *        or equal to `n`.
		 */
		void reserve(size_t n) {
			c.reserve(n);
		}
	};
	/**
	 * @brief Unordered set of A* nodes.
	 */
	typedef std::unordered_set<AstarNodeP, AstarNodeHash, AstarNodeEqual> AstarSet;
	/**
	 * @brief A* "OPEN priority queue" type.
	 */
	class AstarOpen {
	private:
		AstarPrioq m_prioq;
		AstarSet m_set;
	public:
		/**
		 * @brief Pushes the given element to the priority queue.
		 */
		void push(const AstarNodeP& x) {
			m_prioq.push(x);
			m_set.insert(x);
		}
		/**
		 * @brief Removes the top element from the priority queue.
		 */
		void pop() {
			m_set.erase(m_prioq.top());
			m_prioq.pop();
		}
		/**
		 * @brief Returns reference to the top element in the priority queue.
		 */
		const AstarNodeP& top() const {
			return m_prioq.top();
		}
		/**
		 * @brief Checks if the container contains the given element.
		 */
		bool contains(const AstarNodeP& x) const {
			return (m_set.find(x) != m_set.end());
		}
		/**
		 * @brief Returns the number of elements in the container.
		 */
		size_t size() const {
			return m_prioq.size();
		}
		/**
		 * @brief Checks whether the container is empty.
		 */
		bool empty() const {
			return m_prioq.empty();
		}
		/**
		 * @brief Increases the capacity of the container to a value greater
		 *        or equal to `n`.
		 */
		void reserve(size_t n) {
			m_prioq.reserve(n);
			m_set.reserve(n);
		}
	};
	/**
	 * @brief A* "CLOSED set" type.
	 */
	class AstarClosed : public AstarSet {
	public:
		/**
		 * @brief Checks if the container contains the given element.
		 */
		bool contains(const AstarNodeP& x) const {
			return (find(x) != end());
		}
	};

	struct AstarData {
		// Quick access

		// Player state which belongs to this agent
		const GameStateAgentProxy::PlayerState& me;
		// The player to chase after
		const GameStateAgentProxy::PlayerState* victim;
		const StageGridModel& grid;
		// Square of "my" player radius
		double mySqsize;

		// Initial state
		StageGridModel::Cell sStart;
		// Goal state
		StageGridModel::Cell sGoal;
		
		// OPEN priority queue
		AstarOpen astarOpen;
		// CLOSED set
		AstarClosed astarClosed;
		
		// The maximum number of nodes generated per search. When this value is
		// exceeded, the search must stop.
		static constexpr int MAX_GENERATED_NODES = 2200;
		// The number of nodes generated during this search
		int generatedNodes;

		// Whether the search has finished
		bool isFinished;
		// The best input as chosen by the search
		PlayerInputFlags bestInput;

#if ASTAR_PREDATOR_VERSION == 3
		// The node which has been nearest to the goal during this search
		AstarNodeP nearestNode;
#endif // ASTAR_PREDATOR_VERSION == 3
	};
private:
	// Possible actions
	// The actions are limited to cardinal only, because diagonal movement
	// is more likely to not work correctly.
	static constexpr AstarActions ASTAR_ACTIONS{DIR8_N, DIR8_E, DIR8_S, DIR8_W};

	PlayerInputFlags m_input;

	/**
	 * @brief Chooses which action to take next.
	 * 
	 * @note Performs A* search.
	 * 
	 * @param victim The player to chase after.
	 */
	PlayerInputFlags chooseNextAction(
		const GameStateAgentProxy::PlayerState* victim);
	
	/**
	 * @brief Creates an `AstarData` structure.
	 * 
	 * @param victim The player to chase after.
	 * 
	 * @note The `astarDataInit` method must be called afterwards to properly
	 *       initialize the structure. This method only takes care of its
	 *       successful creation.
	 */
	AstarPredatorAIPlayerAgent::AstarData astarDataCreate(
		const GameStateAgentProxy::PlayerState* victim) const;
	/**
	 * @brief Initializes the values of `AstarData` structure.
	 */
	void astarDataInit(AstarData& d) const;
	/**
	 * @brief Fully processes the node at the top of OPEN.
	 * 
	 * @note May modify `d.isFinished` (besides others). Once the value of this
	 *       variable is set to `true`, the search should not continue.
	 */
	void astarProcessNextNode(AstarData& d) const;
	/**
	 * @brief Performs node "expansion".
	 * 
	 * @param d
	 * @param n The node to expand.
	 */
	void astarExpandNode(AstarData& d, const AstarNodeP& n) const;
	/**
	 * @brief Checks if A* should finish the execution.
	 */
	bool isAstarFinished(AstarData& d) const;
	/**
	 * @brief After A* search chooses the input which would be best made at the
	 *        current state.
	 */
	PlayerInputFlags astarGetBestInput(AstarData& d) const;
#if ASTAR_PREDATOR_VERSION == 3
	/**
	 * @brief Changes `d.nearestNode` for the node at the top of OPEN if it is
	 *        a suitable candidate.
	 * 
	 * @remark OPEN may be empty, in which case the value will not be updated.
	 */
	void astarRefreshNearestNode(AstarData& d) const;
#endif // ASTAR_PREDATOR_VERSION == 3

	/**
	 * @brief Calculates the `h(s)` value of the state `s`.
	 * 
	 * @param s 
	 * @param sGoal Goal state.
	 */
	static AstarPredatorAIPlayerAgent::NodeEval getHvalue(
		const StageGridModel::Cell& s, const StageGridModel::Cell& sGoal);
	/**
	 * @brief Creates successor of the `n` node in the `direction` direction.
	 * 
	 * @param n Node to generate successor for.
	 * @param direction The direction of the successor from the `n` node.
	 * @param sGoal Goal state.
	 */
	static AstarPredatorAIPlayerAgent::AstarNodeP getSucc(
		const AstarPredatorAIPlayerAgent::AstarNodeP& n, Direction8 direction,
		const StageGridModel::Cell& sGoal);

protected:
	void doPlan() override;
	PlayerInputFlags doGetPlayerInput() override { return m_input; }
public:
	AstarPredatorAIPlayerAgent(PlayerId playerId);
};

#endif // ASTARPREDATORAIPLAYERAGENT_HPP
