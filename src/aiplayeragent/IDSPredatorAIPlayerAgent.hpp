/**
 * @file IDSPredatorAIPlayerAgent.hpp
 * @author Tomáš Ludrovan
 * @brief IDSPredatorAIPlayerAgent class
 * @version 0.1
 * @date 2024-04-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef IDSPREDATORAIPLAYERAGENT_HPP
#define IDSPREDATORAIPLAYERAGENT_HPP

#include <memory>
#include <stack>
#include <unordered_set>

#include "playerinput/PlayerInputFlags.hpp"
#include "aiplayeragent/PredatorAIPlayerAgentBase.hpp"
#include "aiplayeragent/StageGridModel.hpp"

class IDSPredatorAIPlayerAgent : public PredatorAIPlayerAgentBase {
private:
	/**
	 * @brief Node in the IDS search tree.
	 */
	struct IDSNode {
		// Cell related to the node.
		StageGridModel::Cell cell;
		// Depth of the node within the tree (0-based).
		int depth;
		// The last action performed upon the node.
		size_t actionIdx;
	};
	/**
	 * @brief Node in the IDS search tree.
	 */
	typedef std::shared_ptr<IDSNode> IDSNodeP;
	/**
	 * @brief Hash function object for the IDS node.
	 */
	struct IDSNodeHash {
		size_t operator()(const IDSNodeP& key) const {
			return StageGridModel::Cell::Hash()(key->cell);
		}
	};
	/**
	 * @brief Equality comparison function object for the IDS node.
	 */
	struct IDSNodeEqual {
		bool operator()(const IDSNodeP& lhs, const IDSNodeP& rhs) const {
			return (lhs->cell.getPosition() == rhs->cell.getPosition());
		}
	};
	typedef std::stack<IDSNodeP> IDSNodePStack;
	typedef std::unordered_set<IDSNodeP, IDSNodeHash, IDSNodeEqual> IDSNodePSet;
	/**
	 * @brief IDS "OPEN stack" type.
	 */
	class IDSOpen {
	private:
		IDSNodePStack m_stack;
		IDSNodePSet m_set;
	public:
		/**
		 * @brief Inserts element at the end of the container.
		 */
		void push(const IDSNodeP& x) {
			m_stack.push(x);
			m_set.insert(x);
		}
		/**
		 * @brief Removes element from the end of the container.
		 */
		void pop() {
			m_set.erase(m_stack.top());
			m_stack.pop();
		}
		/**
		 * @brief Returns reference to the element at the end of the container.
		 */
		const IDSNodeP& top() const {
			return m_stack.top();
		}
		/**
		 * @brief Returns reference to the element at the end of the container.
		 */
		IDSNodeP& top() {
			return m_stack.top();
		}
		/**
		 * @brief Checks if the container contains the given element.
		 */
		bool contains(const IDSNodeP& x) const {
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
	/**
	 * @brief IDS "CLOSED set" type.
	 */
	typedef IDSNodePSet IDSClosed;
private:
	PlayerInputFlags m_input;

	/**
	 * @brief Chooses which action to take next.
	 * 
	 * @note Performs IDS.
	 * 
	 * @param victim The player to chase after.
	 */
	PlayerInputFlags chooseNextAction(
		const GameStateAgentProxy::PlayerState* victim);
protected:
	void doPlan() override;
	PlayerInputFlags doGetPlayerInput() override { return m_input; }
public:
	IDSPredatorAIPlayerAgent(PlayerId playerId);
};

#endif // IDSPREDATORAIPLAYERAGENT_HPP
