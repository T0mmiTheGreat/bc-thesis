/**
 * @file YAMLStageSerializer.cpp
 * @author Tomáš Ludrovan
 * @brief YAMLStageSerializer class
 * @version 0.1
 * @date 2024-02-13
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "stageserializer/YAMLStageSerializer.hpp"

#include <iostream>
#include <filesystem>
#include <fstream>

#include "functions.hpp"

static constexpr auto STAGE_KEY          = "stage";
static constexpr auto TITLE_KEY          = "title";
static constexpr auto WIDTH_KEY          = "width";
static constexpr auto HEIGHT_KEY         = "height";
static constexpr auto PLAYERS_KEY        = "players";
static constexpr auto OBSTACLES_KEY      = "obstacles";
static constexpr auto POSITION_RULES_KEY = "positionRules";

static YAML::Emitter& operator<< (YAML::Emitter& out, 
	const YAMLStageSerializer& value);
static YAML::Emitter& operator<< (YAML::Emitter& out, 
	const YAMLStageSerializer::PlayerType& value);
static YAML::Emitter& operator<< (YAML::Emitter& out, 
	const YAMLStageSerializer::ObstacleType& value);
static YAML::Emitter& operator<< (YAML::Emitter& out, 
	const std::vector<YAMLStageSerializer::PositionRuleType>& value);


void YAMLStageSerializer::throwIOException(const std::string& fn,
	const std::string& what) const
{
	throw IOException("Could not open file " + fn + ": " + what);
}

void YAMLStageSerializer::throwParserException(const std::string& fn,
	const std::string& what) const
{
	throw IOException("Could not read file " + fn + ": " + what);
}

void YAMLStageSerializer::throwParserExceptionMissingKey(
	const std::string& fn, const std::string& key)
{
	throwParserException(fn, concatStrings("Missing key \"", key, "\""));
}

void YAMLStageSerializer::throwParserExceptionInvalidKeyValue(
	const std::string& fn, const std::string& key)
{
	throwParserException(fn, concatStrings("Invalid ", key, " value"));
}

std::string YAMLStageSerializer::getStagePath(
	const IStageSerializer::IdType& id)
{
	return concatStrings(STAGES_PATH, id, FILE_EXT);
}

YAMLStageSerializer::YAMLStageSerializer()
	: StageSerializerBase()
{}

void YAMLStageSerializer::save(const std::string& id) const
{
	try {
		// Make file name
		std::string fn = getStagePath(id);

		// Open file stream
		std::ofstream stream(fn);
		// Enable exceptions
		stream.exceptions(std::ios_base::badbit | std::ios_base::failbit);

		// Generate YAML
		YAML::Emitter out;
		out << *this;
		
		assert(out.good());

		// Output to file
		stream << out.c_str();

	} catch (const std::ios_base::failure& e) {
		throwIOException(id, e.what());
	} catch (const YAML::Exception& e) {
		throwParserException(id, e.what());
	}
}

void YAMLStageSerializer::load(const IStageSerializer::IdType& id)
{
	(void)id;
	// TODO
}

std::unordered_set<IStageSerializer::IdType>
YAMLStageSerializer::getAllIds() const
{
	std::unordered_set<IdType> res;
	IdType id;

	for (auto const& entry : std::filesystem::directory_iterator(STAGES_PATH)) {
		// We assume that every .yaml file in the "stage/" directory is a stage
		if (entry.path().extension() == FILE_EXT) {
			id = entry.path()        // The entry path
				      .filename()    // The entry file name
				      .stem()        // The file name without the extension
				      .string();     // Converted to string
			res.insert(std::move(id));
		}
	}

	return res;
}

std::unordered_map<IStageSerializer::IdType,std::string>
YAMLStageSerializer::getAllTitles() const
{
	// TODO
	return std::unordered_map<IdType,std::string>();
}

YAML::Emitter& operator<< (YAML::Emitter& out,
	const YAMLStageSerializer& value)
{
	out << YAML::BeginMap;
		// stage:
		out << YAML::Key << STAGE_KEY << YAML::Value << YAML::BeginMap;
			// title:
			out << YAML::Key << TITLE_KEY;
			out << YAML::Value << value.getTitle();
			// width:
			out << YAML::Key << WIDTH_KEY;
			out << YAML::Value << value.getWidth();
			// height:
			out << YAML::Key << HEIGHT_KEY;
			out << YAML::Value << value.getHeight();
			// players:
			out << YAML::Key << PLAYERS_KEY;
			out << YAML::Value << value.getPlayers();
			// obstacles:
			out << YAML::Key << OBSTACLES_KEY;
			out << YAML::Value << value.getObstacles();
			// positionRules:
			out << YAML::Key << POSITION_RULES_KEY;
			out << YAML::Value << value.getPositionRules();
		out << YAML::EndMap;
	out << YAML::EndMap;
	
	assert(out.good());
	return out;
}

YAML::Emitter& operator<< (YAML::Emitter& out,
	const YAMLStageSerializer::PlayerType& value)
{
	out << YAML::Flow << YAML::BeginSeq;
		out << value.x;
		out << value.y;
	out << YAML::EndSeq;
	
	assert(out.good());
	return out;
}

YAML::Emitter& operator<< (YAML::Emitter& out,
	const YAMLStageSerializer::ObstacleType& value)
{
	out << YAML::Flow;
	// Corners are vector
	// Corner type is equivalent to `YAMLStageSerializer::PlayerType`, which is
	// implemented here too
	out << value.corners;
	
	assert(out.good());
	return out;
}

YAML::Emitter& operator<< (YAML::Emitter& out,
	const std::vector<YAMLStageSerializer::PositionRuleType>& value)
{
	out << YAML::BeginSeq;
	for (auto rule : value) {
		out << YAML::Flow << rule;
	}
	out << YAML::EndSeq;
	
	assert(out.good());
	return out;
}
