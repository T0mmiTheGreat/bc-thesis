/**
 * @file AIPlayerAgentFactory.cpp
 * @author Tomáš Ludrovan
 * @brief AIPlayerAgentFactory class
 * @version 0.1
 * @date 2024-03-25
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "aiplayeragent/AIPlayerAgentFactory.hpp"

#include "aiplayeragent/LadybugAIPlayerAgent.hpp"

std::shared_ptr<IAIPlayerAgent> AIPlayerAgentFactory::createLadybugAIPlayerAgent()
{
	return std::make_shared<LadybugAIPlayerAgent>();
}
