/**
 * @file AIPlayerAgentFactory.hpp
 * @author Tomáš Ludrovan
 * @brief AIPlayerAgentFactory class
 * @version 0.1
 * @date 2024-03-25
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef AIPLAYERAGENTFACTORY_HPP
#define AIPLAYERAGENTFACTORY_HPP

#include <memory>

#include "aiplayeragent/IAIPlayerAgent.hpp"
#include "core/Common.hpp"

class AIPlayerAgentFactory {
public:
	static std::shared_ptr<IAIPlayerAgent> createLadybugAIPlayerAgent(
		PlayerId playerId);
	static std::shared_ptr<IAIPlayerAgent> createBlindPredatorAIPlayerAgent(
		PlayerId playerId);
	static std::shared_ptr<IAIPlayerAgent> createBlindPreyAIPlayerAgent(
		PlayerId playerId);
	static std::shared_ptr<IAIPlayerAgent> createWallAwarePredatorAIPlayerAgent(
		PlayerId playerId);
};

#endif // AIPLAYERAGENTFACTORY_HPP
