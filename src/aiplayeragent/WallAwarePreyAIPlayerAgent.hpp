/**
 * @file WallAwarePreyAIPlayerAgent.hpp
 * @author Tomáš Ludrovan
 * @brief WallAwarePreyAIPlayerAgent class
 * @version 0.1
 * @date 2024-03-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef WALLAWAREPREYAIPLAYERAGENT_HPP
#define WALLAWAREPREYAIPLAYERAGENT_HPP

#include "aiplayeragent/OneStepLookaheadAIPlayerAgentBase.hpp"
#include "aiplayeragent/PreyAIPlayerAgentBase.hpp"

class WallAwarePreyAIPlayerAgent
	: public OneStepLookaheadAIPlayerAgentBase
	, public PreyAIPlayerAgentBase
{
protected:
	Point_2 calculateNewPosition(const PlayerInputFlags& input) const override;
	double evaluatePlayer(
		const GameStateAgentProxy::PlayerState& player, const Point_2& pos)
		const override;
public:
	WallAwarePreyAIPlayerAgent(PlayerId playerId);
};

#endif // WALLAWAREPREYAIPLAYERAGENT_HPP
