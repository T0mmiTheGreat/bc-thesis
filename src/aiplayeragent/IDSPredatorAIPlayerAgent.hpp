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
	struct IDSNode {
		StageGridModel::Cell cell;
		int depth;
		size_t actionIdx;
	};
	typedef std::shared_ptr<IDSNode> IDSNodeP;
	struct IDSNodeHash {
		size_t operator()(const IDSNodeP& key) const {
			return StageGridModel::Cell::Hash()(key->cell);
		}
	};
	struct IDSNodeEqual {
		bool operator()(const IDSNodeP& lhs, const IDSNodeP& rhs) const {
			return (lhs->cell.getPosition() == rhs->cell.getPosition());
		}
	};
	typedef std::stack<IDSNodeP> IDSNodePStack;
	typedef std::unordered_set<IDSNodeP, IDSNodeHash, IDSNodeEqual> IDSNodePSet;
	class IDSOpen {
	private:
		IDSNodePStack m_stack;
		IDSNodePSet m_set;
	public:
		void push(const IDSNodeP& x) {
			m_stack.push(x);
			m_set.insert(x);
		}
		void pop() {
			m_set.erase(m_stack.top());
			m_stack.pop();
		}
		const IDSNodeP& top() const {
			return m_stack.top();
		}
		IDSNodeP& top() {
			return m_stack.top();
		}
		bool contains(const IDSNodeP& x) const {
			return (m_set.find(x) != m_set.end());
		}
		size_t size() const {
			return m_stack.size();
		}
		bool empty() const {
			return m_stack.empty();
		}
	};
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
