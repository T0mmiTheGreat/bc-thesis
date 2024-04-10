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
	typedef std::priority_queue<AstarNodeP, std::vector<AstarNodeP>,
		AstarNodeCompare> AstarPrioq;
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
	};
	class AstarClosed : public AstarSet {
	public:
		bool contains(const AstarNodeP& x) const {
			return (find(x) != end());
		}
	};
private:
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
