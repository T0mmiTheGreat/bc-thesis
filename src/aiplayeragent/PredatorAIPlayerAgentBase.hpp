/**
 * @file PredatorAIPlayerAgentBase.hpp
 * @author Tomáš Ludrovan
 * @brief PredatorAIPlayerAgentBase class
 * @version 0.1
 * @date 2024-04-06
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef PREDATORAIPLAYERAGENTBASE_HPP
#define PREDATORAIPLAYERAGENTBASE_HPP

#include "aiplayeragent/AIPlayerAgentBase.hpp"

class PredatorAIPlayerAgentBase : virtual public AIPlayerAgentBase {
protected:
	/**
	 * @brief Chooses which player to chase after.
	 * 
	 * @return The chosen player, or `nullptr` if no player can be chosen
	 *         (e.g., due to agent's victory).
	 */
	const GameStateAgentProxy::PlayerState* chooseVictim();
	double evaluatePlayer(const GameStateAgentProxy::PlayerState& player,
		const Point_2& pos) const;
public:
	PredatorAIPlayerAgentBase(PlayerId playerId);
};

#endif // PREDATORAIPLAYERAGENTBASE_HPP
