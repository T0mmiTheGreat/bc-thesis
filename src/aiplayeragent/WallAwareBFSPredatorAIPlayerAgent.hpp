/**
 * @file WallAwareBFSPredatorAIPlayerAgent.hpp
 * @author Tomáš Ludrovan
 * @brief WallAwareBFSPredatorAIPlayerAgent class
 * @version 0.1
 * @date 2024-04-06
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef WALLAWAREBFSPREDATORAIPLAYERAGENT_HPP
#define WALLAWAREBFSPREDATORAIPLAYERAGENT_HPP

#include <queue>
#include <memory>
#include <unordered_set>

#include "aiplayeragent/PredatorAIPlayerAgentBase.hpp"

class WallAwareBFSPredatorAIPlayerAgent : public PredatorAIPlayerAgentBase {
private:
	/**
	 * @brief Breadth-First search tree node.
	 */
	struct BFSNode {
		// Position of the player at the node.
		Point_2 pos;
		// Action required to get to the node from the previous node.
		PlayerInputFlags action;
		// Pointer to the previous node (`nullptr` at the tree root).
		BFSNode* prev;
	};
	/**
	 * @brief Breadth-First search tree node pointer.
	 */
	typedef std::shared_ptr<BFSNode> BFSNodeP;
	/**
	 * @brief Hash function object for BFSNodeP.
	 */
	struct BFSNodeHash {
		size_t operator() (const BFSNodeP& key) const {
			return CGAL::hash_value(key->pos);
		}
	};
	/**
	 * @brief Equality function object for BFSNodeP.
	 */
	struct BFSNodeEqual {
		bool operator() (const BFSNode* lhs, const BFSNodeP& rhs) const {
			return (lhs->pos == rhs->pos);
		}
		bool operator() (const BFSNodeP& lhs, const BFSNodeP& rhs) const {
			return (*this)(lhs.get(), rhs);
		}
	};

	typedef std::queue<BFSNodeP> BFSNodePQueue;
	typedef std::unordered_set<BFSNodeP, BFSNodeHash, BFSNodeEqual> BFSNodePSet;
	
	/**
	 * @brief BFS "OPEN queue" type.
	 * 
	 * @details 2 in 1: a queue and a set. 
	 */
	class BFSOpen {
	private:
		BFSNodePQueue m_openQueue;
		BFSNodePSet m_openSet;
	public:
		/**
		 * @brief Returns iterator to the first element of the container.
		 * 
		 * @note The iterator is read-only, i.e., you cannot change the elements
		 *       in the container the iterator.
		 */
		decltype(auto) begin() {
			return m_openSet.begin();
		}
		/**
		 * @brief Returns const iterator to the first element of the container.
		 */
		decltype(auto) begin() const {
			return m_openSet.begin();
		}
		/**
		 * @brief Returns iterator following the last element of the container.
		 */
		decltype(auto) end() {
			return m_openSet.end();
		}
		/**
		 * @brief Returns const iterator following the last element of the
		 *        container.
		 */
		decltype(auto) end() const {
			return m_openSet.end();
		}

		/**
		 * @brief Inserts element at the end of the container.
		 */
		void push(const BFSNodeP& x) {
			m_openQueue.push(x);
			m_openSet.insert(x);
		}
		/**
		 * @brief Removes element from the front of the container.
		 */
		void pop() {
			// `item = m_openQueue.pop()`
			auto item = m_openQueue.front();
			m_openQueue.pop();
			
			m_openSet.erase(item);
		}
		/**
		 * @brief Returns reference to the element at the front of the
		 *        container.
		 */
		decltype(auto) front() {
			return m_openQueue.front();
		}
		/**
		 * @brief Returns const reference to the element at the front of the
		 *        container.
		 */
		decltype(auto) front() const {
			return m_openQueue.front();
		}

		/**
		 * @brief Finds an element which compares equal to `key`.
		 * 
		 * @return Iterator to the requested element, or `end()` if no such
		 *         element exists in the container.
		 */
		decltype(auto) find(const BFSNodeP& key) {
			return m_openSet.find(key);
		}
		/**
		 * @brief Finds an element which compares equal to `key`.
		 * 
		 * @return Const iterator to the requested element, or `end()` if no
		 *         such element exists in the container.
		 */
		decltype(auto) find(const BFSNodeP& key) const {
			return m_openSet.find(key);
		}

		/**
		 * @brief Returns the number of elements in the container.
		 */
		size_t size() const {
			return m_openQueue.size();
		}
		/**
		 * @brief Checks whether the container is empty.
		 */
		bool empty() const {
			return m_openQueue.empty();
		}
	};
	/**
	 * @brief BFS "CLOSED set" type.
	 */
	typedef BFSNodePSet BFSClosed;
private:
	PlayerInputFlags m_input;

	/**
	 * @brief Chooses which player to chase after.
	 * 
	 * @return The chosen player, or `nullptr` if no player can be chosen
	 *         (e.g., due to agent's victory).
	 */
	const GameStateAgentProxy::PlayerState* chooseVictim();
	/**
	 * @brief Chooses which action to take next.
	 * 
	 * @note Performs BFS.
	 * 
	 * @param victim The player to chase after.
	 */
	PlayerInputFlags chooseNextAction(
		const GameStateAgentProxy::PlayerState* victim);
	
	/**
	 * @brief Checks if a node is a goal state of BFS.
	 * 
	 * @param n The node to check.
	 * @param me Player state which belongs to this agent.
	 * @param victim The player to chase after.
	 */
	bool bfsGoalTest(const WallAwareBFSPredatorAIPlayerAgent::BFSNodeP& n,
		const GameStateAgentProxy::PlayerState& me,
		const GameStateAgentProxy::PlayerState* victim);
	/**
	 * @brief Creates an immediate successor of a node.
	 * 
	 * @param n The node for which the successor should be created.
	 * @param action The action which should be taken to get to the successor.
	 * @param me Player state which belongs to this agent.
	 */
	WallAwareBFSPredatorAIPlayerAgent::BFSNodeP bfsCreateSucc(
		WallAwareBFSPredatorAIPlayerAgent::BFSNodeP& n,
		const PlayerInputFlags& action,
		const GameStateAgentProxy::PlayerState& me);
	/**
	 * @brief Checks if node is unique.
	 * 
	 * @note Node is unique if it doesn't exist in neither the OPEN nor CLOSED
	 *       containers.
	 * 
	 * @param n The node to check.
	 * @param bfsOpen OPEN queue.
	 * @param bfsClosed CLOSED set.
	 * @return 
	 */
	bool bfsNodeExists(WallAwareBFSPredatorAIPlayerAgent::BFSNodeP& n,
		const WallAwareBFSPredatorAIPlayerAgent::BFSOpen& bfsOpen,
		const WallAwareBFSPredatorAIPlayerAgent::BFSClosed& bfsClosed);
protected:
	void doPlan() override;
	PlayerInputFlags doGetPlayerInput() override { return m_input; }
public:
	WallAwareBFSPredatorAIPlayerAgent(PlayerId playerId);
};

#endif // WALLAWAREBFSPREDATORAIPLAYERAGENT_HPP
