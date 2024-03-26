/**
 * @file GameSetupData.hpp
 * @author Tomáš Ludrovan
 * @brief GameSetupData struct
 * @version 0.1
 * @date 2024-03-17
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef GAMESETUPDATA_HPP
#define GAMESETUPDATA_HPP

#include <memory>
#include <vector>

#include "aiplayeragent/IAIPlayerAgent.hpp"
#include "playerinput/IPlayerInput.hpp"
#include "stageserializer/IStageSerializer.hpp"

struct GameSetupData {
	std::shared_ptr<IStageSerializer> stage;
	std::vector<std::shared_ptr<IPlayerInput>> players;
	std::vector<std::shared_ptr<IAIPlayerAgent>> aiAgents;
};

#endif // GAMESETUPDATA_HPP
