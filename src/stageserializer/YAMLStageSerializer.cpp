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

// Conversions between `YAML::Node` and custom data types
namespace YAML {

namespace ErrorMsg {
const char* const POSITION_RULE =
	"invalid position rule";
}

class InvalidPositionRule : public RepresentationException {
public:
	InvalidPositionRule(const Mark& mark)
		: RepresentationException(mark, ErrorMsg::POSITION_RULE)
	{}
	InvalidPositionRule(const InvalidPositionRule&) = default;
	~InvalidPositionRule() noexcept override = default;
};

// IStageSerializer::PlayerType
template<>
struct convert<IStageSerializer::PlayerType> {
	static Node encode(const IStageSerializer::PlayerType& rhs) {
		Node node;
		node.push_back(rhs.x);
		node.push_back(rhs.y);
		return node;
	}

	static bool decode(const Node& node, IStageSerializer::PlayerType& rhs) {
		if (!node.IsSequence() || node.size() != 2) {
			return false;
		}

		rhs.x = node[0].as<IStageSerializer::PlayerType::ValueType>();
		rhs.y = node[1].as<IStageSerializer::PlayerType::ValueType>();
		return true;
	}
};

// IStageSerializer::ObstacleType
template<>
struct convert<IStageSerializer::ObstacleType> {
	static Node encode(const IStageSerializer::ObstacleType& rhs) {
		Node node;
		for (const auto& corner : rhs.corners) {
			// Corner type is equivalent to `YAMLStageSerializer::PlayerType`,
			// which is implemented here too
			node.push_back(corner);
		}
		return node;
	}

	static bool decode(const Node& node, IStageSerializer::ObstacleType& rhs) {
		if (!node.IsSequence() || node.size() != rhs.CORNER_COUNT) {
			return false;
		}

		for (size_t i = 0; i < rhs.CORNER_COUNT; i++) {
			rhs.corners[i] = node[i].as<IStageSerializer::ObstacleType::CornerType>();
		}
		return true;
	}
};

// YAMLStageSerializer
template<>
struct convert<YAMLStageSerializer> {
	static Node encode(const YAMLStageSerializer& rhs) {
		Node res, stage;
		
		stage[TITLE_KEY] = rhs.getTitle();
		stage[WIDTH_KEY] = rhs.getWidth();
		stage[HEIGHT_KEY] = rhs.getHeight();
		stage[PLAYERS_KEY] = rhs.getPlayers();
		stage[OBSTACLES_KEY] = rhs.getObstacles();
		stage[POSITION_RULES_KEY] = rhs.getPositionRules();
		
		res[STAGE_KEY] = std::move(stage);

		return res;
	}

	static Node getNode(const Node& parent, const std::string& key) {
		auto res = parent[key];
		if (!res) throw KeyNotFound(parent.Mark(), key);
		return res;
	}

	static std::vector<IStageSerializer::PositionRuleType>
	nodeAsPositionRuleVector(const Node& node, size_t playersCount) {
		auto res = node.as<std::vector<IStageSerializer::PositionRuleType>>();
		// Must check that all values are valid
		for (const auto& rule : res) {
			// Check if it contains duplicates
			if (vectorHasDuplicates(rule)) {
				throw InvalidPositionRule(node.Mark());
			}
			// Check if all indexes are within the player list
			for (const auto& index : rule) {
				if (!(0 <= index && index < static_cast<int>(playersCount))) {
					throw InvalidPositionRule(node.Mark());
				}
			}
		}

		return res;
	}

	static bool decode(const Node& node, YAMLStageSerializer& rhs) {
		if (!node.IsMap()) return false;

		// Read stage node
		auto stageNode = getNode(node, STAGE_KEY);
		if (!stageNode.IsMap()) return false;

		// Read the children
		auto titleNode = getNode(stageNode, TITLE_KEY);
		auto widthNode = getNode(stageNode, WIDTH_KEY);
		auto heightNode = getNode(stageNode, HEIGHT_KEY);
		auto playersNode = getNode(stageNode, PLAYERS_KEY);
		auto obstaclesNode = getNode(stageNode, OBSTACLES_KEY);
		auto positionRulesNode = getNode(stageNode, POSITION_RULES_KEY);

		// title:
		auto title = titleNode.as<std::string>();
		rhs.setTitle(std::move(title));
		// width:
		auto width = widthNode.as<int>();
		rhs.setWidth(std::move(width));
		// height:
		auto height = heightNode.as<int>();
		rhs.setHeight(std::move(height));
		// players:
		auto players =
			playersNode.as<std::vector<IStageSerializer::PlayerType>>();
		rhs.setPlayers(std::move(players));
		// obstacles:
		auto obstacles =
			obstaclesNode.as<std::vector<IStageSerializer::ObstacleType>>();
		rhs.setObstacles(std::move(obstacles));
		// positionRules:
		auto positionRules = nodeAsPositionRuleVector(positionRulesNode,
			rhs.getPlayers().size());
		rhs.setPositionRules(std::move(positionRules));

		return true;
	}
};
}

static YAML::Emitter& operator<< (YAML::Emitter& out, 
	const YAMLStageSerializer& value);
static YAML::Emitter& operator<< (YAML::Emitter& out, 
	const YAMLStageSerializer::PlayerType& value);
static YAML::Emitter& operator<< (YAML::Emitter& out, 
	const YAMLStageSerializer::ObstacleType& value);
static YAML::Emitter& operator<< (YAML::Emitter& out, 
	const std::vector<YAMLStageSerializer::PositionRuleType>& value);


void YAMLStageSerializer::throwIOException(const std::string& fn,
	const std::string& what)
{
	throw IOException("Could not open file " + fn + ": " + what);
}

void YAMLStageSerializer::throwParserException(const std::string& fn,
	const std::string& what)
{
	throw IOException("Could not read file " + fn + ": " + what);
}

std::string YAMLStageSerializer::getStagePath(
	const IStageSerializer::IdType& id)
{
	return concatStrings(STAGES_PATH, id, FILE_EXT);
}

YAMLStageSerializer::YAMLStageSerializer()
	: StageSerializerBase()
{}

void YAMLStageSerializer::save(const IStageSerializer::IdType& id) const
{
	// Make file name
	std::string fn = getStagePath(id);

	try {

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
		throwIOException(fn, e.what());
	} catch (const YAML::Exception& e) {
		throwParserException(fn, e.what());
	}
}

void YAMLStageSerializer::load(const IStageSerializer::IdType& id)
{
	// Make file name
	std::string fn = getStagePath(id);

	try {
		// Load
		YAML::Node root = YAML::LoadFile(fn);

		// Parse
		auto stage = root.as<YAMLStageSerializer>();
		*this = std::move(stage);

	} catch (const std::ios_base::failure& e) {
		throwIOException(fn, e.what());
	} catch (const YAML::Exception& e) {
		throwParserException(fn, e.what());
	}
	
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

std::string YAMLStageSerializer::getStageCanonicalId(
	const IStageSerializer::IdType& stageId) const
{
	return getStagePath(stageId);
}

YAML::Emitter& operator<< (YAML::Emitter& out,
	const YAMLStageSerializer& value)
{
	out << YAML::BeginMap;
		// stage:
		out << YAML::Key << STAGE_KEY << YAML::Value << YAML::BeginMap;
			// title:
			out << YAML::Key << TITLE_KEY;
			out << YAML::Value;
			out << value.getTitle();
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
	out << YAML::Flow << YAML::BeginSeq;
	for (const auto& corner : value.corners) {
		// Corner type is equivalent to `YAMLStageSerializer::PlayerType`, which
		// is implemented here too
		out << corner;
	}
	out << YAML::EndSeq;
	
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
