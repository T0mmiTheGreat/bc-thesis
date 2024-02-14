/**
 * @file StageSerializerBase.cpp
 * @author Tomáš Ludrovan
 * @brief StageSerializerBase class
 * @version 0.1
 * @date 2024-02-13
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "stageserializer/StageSerializerBase.hpp"

StageSerializerBase::StageSerializerBase()
	: title()
	, width{0}
	, height{0}
	, players()
	, obstacles()
	, positionRules()
{}

IStageSerializer::IdType StageSerializerBase::generateIdByTitle(
	const std::string& title) const
{
	static constexpr size_t MAX_GENERATED_FN_LEN = 28;

	IdType baseName, res;

	for (char c : title) {
		if (isalnum(c)) {
			// Lowercase letters and digits are copied.
			// Uppercase are converted to lowercase.

			baseName.push_back(tolower(c));
		} else if (c == ' ' || c == '_') {
			// Underscores are copied.
			// Spaces are converted to underscores.

			baseName.push_back('_');
		}
		// Other characters are ignored
	}

	if (baseName.empty()) {
		// If the stage name cannot be converted to a non-empty string, default
		// "stage" is used 

		baseName = "stage";
	} else if (baseName.size() > MAX_GENERATED_FN_LEN) {
		baseName.resize(MAX_GENERATED_FN_LEN);
	}

	// Resolve collisions with existing IDs

	unsigned suffix = 0;
	// Existing IDs
	auto ids = getAllIds();
	// First, try it without the suffix
	res = baseName;
	while (ids.contains(res)) {
		suffix++;
		// Add suffix
		res = baseName + std::to_string(suffix);
	}

	return res;
}

const std::string& StageSerializerBase::getTitle() const
{
	return title;
}

void StageSerializerBase::setTitle(const std::string& value)
{
	title = value;
}

int StageSerializerBase::getWidth() const
{
	return width;
}

void StageSerializerBase::setWidth(int value)
{
	width = value;
}

int StageSerializerBase::getHeight() const
{
	return height;
}

void StageSerializerBase::setHeight(int value)
{
	height = value;
}

const std::vector<IStageSerializer::PlayerType>&
StageSerializerBase::getPlayers() const
{
	return players;
}

void StageSerializerBase::setPlayers(
	const std::vector<IStageSerializer::PlayerType>& value)
{
	players = value;
}

const std::vector<IStageSerializer::ObstacleType>&
StageSerializerBase::getObstacles() const
{
	return obstacles;
}

void StageSerializerBase::setObstacles(
	const std::vector<IStageSerializer::ObstacleType>& value)
{
	obstacles = value;
}

const std::vector<IStageSerializer::PositionRuleType>&
StageSerializerBase::getPositionRules() const
{
	return positionRules;
}

void StageSerializerBase::setPositionRules(
	const std::vector<IStageSerializer::PositionRuleType>& value)
{
	positionRules = value;
}
