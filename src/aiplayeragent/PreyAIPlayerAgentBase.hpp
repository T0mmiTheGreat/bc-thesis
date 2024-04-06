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
	double evaluatePlayer(const GameStateAgentProxy::PlayerState& player,
		const Point_2& pos) const;
public:
	PreyAIPlayerAgentBase(PlayerId playerId);
};

#endif // PREYAIPLAYERAGENTBASE_HPP
