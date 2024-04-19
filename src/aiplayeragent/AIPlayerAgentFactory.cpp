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
#include "aiplayeragent/BlindPredatorAIPlayerAgent.hpp"
#include "aiplayeragent/BlindPreyAIPlayerAgent.hpp"
#include "aiplayeragent/WallAwarePredatorAIPlayerAgent.hpp"
#include "aiplayeragent/WallAwarePreyAIPlayerAgent.hpp"
#include "aiplayeragent/WallAwareBFSPredatorAIPlayerAgent.hpp"
#include "aiplayeragent/IDSPredatorAIPlayerAgent.hpp"
#include "aiplayeragent/BFSPredatorAIPlayerAgent.hpp"
#include "aiplayeragent/AstarPredatorAIPlayerAgent.hpp"
#include "aiplayeragent/MinimaxPreyAIPlayerAgent.hpp"

std::shared_ptr<IAIPlayerAgent>
AIPlayerAgentFactory::createLadybugAIPlayerAgent(PlayerId playerId)
{
	return std::make_shared<LadybugAIPlayerAgent>(playerId);
}

std::shared_ptr<IAIPlayerAgent>
AIPlayerAgentFactory::createBlindPredatorAIPlayerAgent(PlayerId playerId)
{
	return std::make_shared<BlindPredatorAIPlayerAgent>(playerId);
}

std::shared_ptr<IAIPlayerAgent>
AIPlayerAgentFactory::createBlindPreyAIPlayerAgent(PlayerId playerId)
{
	return std::make_shared<BlindPreyAIPlayerAgent>(playerId);
}

std::shared_ptr<IAIPlayerAgent>
AIPlayerAgentFactory::createWallAwarePredatorAIPlayerAgent(PlayerId playerId)
{
	return std::make_shared<WallAwarePredatorAIPlayerAgent>(playerId);
}

std::shared_ptr<IAIPlayerAgent>
AIPlayerAgentFactory::createWallAwarePreyAIPlayerAgent(PlayerId playerId)
{
	return std::make_shared<WallAwarePreyAIPlayerAgent>(playerId);
}

std::shared_ptr<IAIPlayerAgent>
AIPlayerAgentFactory::createWallAwareBFSPredatorAIPlayerAgent(PlayerId playerId)
{
	return std::make_shared<WallAwareBFSPredatorAIPlayerAgent>(playerId);
}

std::shared_ptr<IAIPlayerAgent>
AIPlayerAgentFactory::createIDSPredatorAIPlayerAgent(PlayerId playerId)
{
	return std::make_shared<IDSPredatorAIPlayerAgent>(playerId);
}

std::shared_ptr<IAIPlayerAgent>
AIPlayerAgentFactory::createBFSPredatorAIPlayerAgent(PlayerId playerId)
{
	return std::make_shared<BFSPredatorAIPlayerAgent>(playerId);
}

std::shared_ptr<IAIPlayerAgent>
AIPlayerAgentFactory::createAstarPredatorAIPlayerAgent(PlayerId playerId)
{
	return std::make_shared<AstarPredatorAIPlayerAgent>(playerId);
}

std::shared_ptr<IAIPlayerAgent>
AIPlayerAgentFactory::createMinimaxPreyAIPlayerAgent(PlayerId playerId)
{
	return std::make_shared<MinimaxPreyAIPlayerAgent>(playerId);
}
