/**
 * @file BFSPredatorAIPlayerAgent.hpp
 * @author Tomáš Ludrovan
 * @brief BFSPredatorAIPlayerAgent class
 * @version 0.1
 * @date 2024-04-09
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef BFSPREDATORAIPLAYERAGENT_HPP
#define BFSPREDATORAIPLAYERAGENT_HPP

#include <memory>
#include <queue>
#include <unordered_set>

#include "playerinput/PlayerInputFlags.hpp"
#include "aiplayeragent/PredatorAIPlayerAgentBase.hpp"
#include "aiplayeragent/StageGridModel.hpp"

class BFSPredatorAIPlayerAgent : public PredatorAIPlayerAgentBase {
private:
	/**
	 * @brief Node in the BFS search tree.
	 */
	struct BFSNode {
		StageGridModel::Cell cell;
		PlayerInputFlags action;
		BFSNode* prev;
	};
	/**
	 * @brief Node in the BFS search tree.
	 */
	typedef std::shared_ptr<BFSNode> BFSNodeP;
	/**
	 * @brief Hash function object for the BFS node.
	 */
	struct BFSNodeHash {
		size_t operator()(const BFSNodeP& key) const {
			return StageGridModel::Cell::Hash()(key->cell);
		}
	};
	/**
	 * @brief Equality comparison function object for the BFS node.
	 */
	struct BFSNodeEqual {
		bool operator()(const BFSNodeP& lhs, const BFSNodeP& rhs) const {
			return (lhs->cell.getPosition() == rhs->cell.getPosition());
		}
	};
	typedef std::queue<BFSNodeP> BFSNodePQueue;
	typedef std::unordered_set<BFSNodeP, BFSNodeHash, BFSNodeEqual> BFSNodePSet;
	/**
	 * @brief BFS "OPEN queue" type.
	 */
	class BFSOpen {
	private:
		BFSNodePQueue m_stack;
		BFSNodePSet m_set;
	public:
		/**
		 * @brief Inserts element at the end of the container.
		 */
		void push(const BFSNodeP& x) {
			m_stack.push(x);
			m_set.insert(x);
		}
		/**
		 * @brief Removes element from the front of the container.
		 */
		void pop() {
			m_set.erase(m_stack.front());
			m_stack.pop();
		}
		/**
		 * @brief Returns reference to the element at the front of the
		 *        container.
		 */
		const BFSNodeP& front() const {
			return m_stack.front();
		}
		/**
		 * @brief Returns reference to the element at the front of the
		 *        container.
		 */
		BFSNodeP& front() {
			return m_stack.front();
		}
		/**
		 * @brief Checks if the container contains the given element.
		 */
		bool contains(const BFSNodeP& x) const {
			return (m_set.find(x) != m_set.end());
		}
		/**
		 * @brief Returns the number of elements in the container.
		 */
		size_t size() const {
			return m_stack.size();
		}
		/**
		 * @brief Checks whether the container is empty.
		 */
		bool empty() const {
			return m_stack.empty();
		}
	};
	typedef BFSNodePSet BFSClosed;
private:
	PlayerInputFlags m_input;

	/**
	 * @brief Chooses which action to take next.
	 * 
	 * @note Performs BFS.
	 * 
	 * @param victim The player to chase after.
	 */
	PlayerInputFlags chooseNextAction(
		const GameStateAgentProxy::PlayerState* victim);
protected:
	void doPlan() override;
	PlayerInputFlags doGetPlayerInput() override { return m_input; }
public:
	BFSPredatorAIPlayerAgent(PlayerId playerId);
};

#endif // BFSPREDATORAIPLAYERAGENT_HPP
