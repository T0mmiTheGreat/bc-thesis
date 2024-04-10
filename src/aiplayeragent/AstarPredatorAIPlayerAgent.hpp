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

#define ASTAR_PREDATOR_VERSION 2

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
		// f(s)
		NodeEval fvalue;
		// g(s)
		NodeEval gvalue;
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
			return (lhs->fvalue > rhs->fvalue);
		}
	};
	class AstarPrioq
		: public std::priority_queue<AstarNodeP, std::vector<AstarNodeP>,
		  AstarNodeCompare>
	{
	public:
		void reserve(size_t n) {
			c.reserve(n);
		}
	};
	typedef std::unordered_set<AstarNodeP, AstarNodeHash, AstarNodeEqual> AstarSet;
	class AstarOpen {
	private:
		AstarPrioq m_prioq;
		AstarSet m_set;
	public:
		void push(const AstarNodeP& x) {
			m_prioq.push(x);
			m_set.insert(x);
		}
		void pop() {
			m_set.erase(m_prioq.top());
			m_prioq.pop();
		}
		const AstarNodeP& top() const {
			return m_prioq.top();
		}
		bool contains(const AstarNodeP& x) const {
			return (m_set.find(x) != m_set.end());
		}
		size_t size() const {
			return m_prioq.size();
		}
		bool empty() const {
			return m_prioq.empty();
		}
		void reserve(size_t n) {
			m_prioq.reserve(n);
			m_set.reserve(n);
		}
	};
	class AstarClosed : public AstarSet {
	public:
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
		
		AstarOpen astarOpen;
		AstarClosed astarClosed;
		
		static constexpr int MAX_GENERATED_NODES = 2200;
		int generatedNodes;

		bool isFinished;
		PlayerInputFlags bestInput;
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

	static AstarPredatorAIPlayerAgent::NodeEval getHvalue(
		const StageGridModel::Cell& s, const StageGridModel::Cell& sGoal);
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
