/**
 * @file AIPlayerInput.hpp
 * @author Tomáš Ludrovan
 * @brief AIPlayerInput class
 * @version 0.1
 * @date 2024-02-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef AIPLAYERINPUT_HPP
#define AIPLAYERINPUT_HPP

#include <memory>

#include "aiplayeragent/IAIPlayerAgent.hpp"
#include "playerinput/PlayerInputBase.hpp"

class AIPlayerInput : public PlayerInputBase {
private:
	std::shared_ptr<IAIPlayerAgent> m_agent;
public:
	AIPlayerInput(std::shared_ptr<IAIPlayerAgent> agent);
	PlayerInputFlags readInput() override;
};

#endif // AIPLAYERINPUT_HPP
