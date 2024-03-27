/**
 * @file WallAwarePredatorAIPlayerAgent.hpp
 * @author Tomáš Ludrovan
 * @brief WallAwarePredatorAIPlayerAgent class
 * @version 0.1
 * @date 2024-03-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef WALLAWAREPREDATORAIPLAYERAGENT_HPP
#define WALLAWAREPREDATORAIPLAYERAGENT_HPP

#include "aiplayeragent/OneStepLookaheadAIPlayerAgentBase.hpp"

class WallAwarePredatorAIPlayerAgent : public OneStepLookaheadAIPlayerAgentBase {
protected:
	Point_2 calculateNewPosition(const PlayerInputFlags& input) const override;
	double evaluatePlayer(
		const GameStateAgentProxy::PlayerState& player, const Point_2& pos)
		const override;
public:
	WallAwarePredatorAIPlayerAgent(PlayerId playerId);
};

#endif // WALLAWAREPREDATORAIPLAYERAGENT_HPP
