/**
 * @file BlindPreyAIPlayerAgent.hpp
 * @author Tomáš Ludrovan
 * @brief BlindPreyAIPlayerAgent class
 * @version 0.1
 * @date 2024-03-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef BLINDPREYAIPLAYERAGENT_HPP
#define BLINDPREYAIPLAYERAGENT_HPP

#include "aiplayeragent/OneStepLookaheadAIPlayerAgentBase.hpp"
#include "aiplayeragent/PreyAIPlayerAgentBase.hpp"

class BlindPreyAIPlayerAgent
	: public OneStepLookaheadAIPlayerAgentBase
	, public PreyAIPlayerAgentBase
{
protected:
	Point_2 calculateNewPosition(const PlayerInputFlags& input) const override;
	double evaluatePlayer(
		const GameStateAgentProxy::PlayerState& player, const Point_2& pos)
		const override;
public:
	BlindPreyAIPlayerAgent(PlayerId playerId);
};

#endif // BLINDPREYAIPLAYERAGENT_HPP
