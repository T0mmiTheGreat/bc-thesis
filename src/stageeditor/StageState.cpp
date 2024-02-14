/**
 * @file StageState.cpp
 * @author Tomáš Ludrovan
 * @brief StageState structure
 * @version 0.1
 * @date 2024-01-26
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "stageeditor/StageState.hpp"

#include "stageserializer/StageSerializerFactory.hpp"

std::shared_ptr<IStageSerializer> StageState::serialize() const
{
	auto res = StageSerializerFactory::createDefault();
	// Title
	res->setTitle(title);
	// Width
	res->setWidth(width);
	// Height
	res->setHeight(height);

	// Players -- `this->players` as a vector
	std::vector<IStageSerializer::PlayerType> players_;
	players_.reserve(players.size());
	for (const auto& playerPair : players) {
		const auto& player = playerPair.second;
		players_.push_back(player.pos);
	}
	res->setPlayers(players_);

	// Obstacles -- `this->obstacles` as a vector
	std::vector<IStageSerializer::ObstacleType> obstacles_;
	obstacles_.reserve(obstacles.size());
	for (const auto& obstaclePair : obstacles) {
		const auto& obstacle = obstaclePair.second;
		obstacles_.push_back(obstacle.shape);
	}
	res->setObstacles(obstacles_);

	// Position rules -- just a sequence of numbers:
	// `0 -> players_.size() - 1`
	// FIXME: Editor should allow making custom position rules

	// The list
	std::vector<IStageSerializer::PositionRuleType> positionRules_;
	// A single rule
	IStageSerializer::PositionRuleType positionRule_;
	positionRule_.reserve(players_.size());
	// Create the rule
	for (size_t i = 0; i < players_.size(); i++) positionRule_.push_back(i);
	// Insert to the rules list
	positionRules_.push_back(std::move(positionRule_));
	res->setPositionRules(positionRules_);

	return res;
}
