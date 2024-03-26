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

class AIPlayerAgentFactory {
public:
	static std::shared_ptr<IAIPlayerAgent> createLadybugAIPlayerAgent();
};

#endif // AIPLAYERAGENTFACTORY_HPP
