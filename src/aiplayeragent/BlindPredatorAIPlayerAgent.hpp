/**
 * @file BlindPredatorAIPlayerAgent.hpp
 * @author Tomáš Ludrovan
 * @brief BlindPredatorAIPlayerAgent class
 * @version 0.1
 * @date 2024-03-26
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef BLINDPREDATORAIPLAYERAGENT_HPP
#define BLINDPREDATORAIPLAYERAGENT_HPP

#include "aiplayeragent/OneStepLookaheadAIPlayerAgentBase.hpp"

class BlindPredatorAIPlayerAgent : public OneStepLookaheadAIPlayerAgentBase {
protected:
	Point_2 calculateNewPosition(const PlayerInputFlags& input) const override;
	double evaluatePlayer(
		const GameStateAgentProxy::PlayerState& player, const Point_2& pos)
		const override;
public:
	BlindPredatorAIPlayerAgent(PlayerId playerId);
};

#endif // BLINDPREDATORAIPLAYERAGENT_HPP
