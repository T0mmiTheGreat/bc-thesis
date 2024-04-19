/**
 * @file PreyAIPlayerAgentBase.hpp
 * @author Tomáš Ludrovan
 * @brief PreyAIPlayerAgentBase class
 * @version 0.1
 * @date 2024-04-06
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef PREYAIPLAYERAGENTBASE_HPP
#define PREYAIPLAYERAGENTBASE_HPP

#include "aiplayeragent/AIPlayerAgentBase.hpp"

class PreyAIPlayerAgentBase : virtual public AIPlayerAgentBase {
protected:
	/**
	 * @brief Chooses opponent which poses the biggest threat.
	 * 
	 * @return The chosen player, or `nullptr` if no player can be chosen
	 *         (e.g., due to agent's victory).
	 */
	const GameStateAgentProxy::PlayerState* chooseAttacker();
	double evaluatePlayer(const GameStateAgentProxy::PlayerState& player,
		const Point_2& pos) const;
public:
	PreyAIPlayerAgentBase(PlayerId playerId);
};

#endif // PREYAIPLAYERAGENTBASE_HPP
