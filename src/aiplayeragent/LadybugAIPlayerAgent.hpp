/**
 * @file LadybugAIPlayerAgent.hpp
 * @author Tomáš Ludrovan
 * @brief LadybugAIPlayerAgent class
 * @version 0.1
 * @date 2024-03-26
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef LADYBUGAIPLAYERAGENT_HPP
#define LADYBUGAIPLAYERAGENT_HPP

#include "aiplayeragent/AIPlayerAgentBase.hpp"

class LadybugAIPlayerAgent : public AIPlayerAgentBase {
private:
	PlayerInputFlags m_input;
protected:
	void doPlan() override;
	PlayerInputFlags doGetPlayerInput() override;
public:
	LadybugAIPlayerAgent(PlayerId playerId);
};

#endif // LADYBUGAIPLAYERAGENT_HPP
