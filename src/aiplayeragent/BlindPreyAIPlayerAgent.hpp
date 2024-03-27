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

#include <vector>

#include "types.hpp"
#include "aiplayeragent/AIPlayerAgentBase.hpp"
#include "playerinput/PlayerInputFlags.hpp"

class BlindPreyAIPlayerAgent : public AIPlayerAgentBase {
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
	BlindPreyAIPlayerAgent(PlayerId playerId);
};

#endif // BLINDPREYAIPLAYERAGENT_HPP
