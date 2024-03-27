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

#include <vector>

#include "types.hpp"
#include "aiplayeragent/AIPlayerAgentBase.hpp"
#include "playerinput/PlayerInputFlags.hpp"

class BlindPredatorAIPlayerAgent : public AIPlayerAgentBase {
private:
	PlayerInputFlags m_input;

	static std::vector<PlayerInputFlags> generateInputs();
	double evaluatePosition(const PointF& pos) const;
	static double evaluatePlayer(const GameStateAgentProxy::PlayerState& player,
		const PointF& pos);
protected:
	void doPlan() override;
	PlayerInputFlags doGetPlayerInput() override;
public:
	BlindPredatorAIPlayerAgent(PlayerId playerId);
};

#endif // BLINDPREDATORAIPLAYERAGENT_HPP
