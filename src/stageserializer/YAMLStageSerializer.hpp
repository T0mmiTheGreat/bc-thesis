/**
 * @file YAMLStageSerializer.hpp
 * @author Tomáš Ludrovan
 * @brief YAMLStageSerializer class
 * @version 0.1
 * @date 2024-02-13
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef YAMLSTAGESERIALIZER_HPP
#define YAMLSTAGESERIALIZER_HPP

#include <yaml-cpp/yaml.h>

#include "stageserializer/StageSerializerBase.hpp"

class YAMLStageSerializer : public StageSerializerBase {
private:
	static constexpr auto STAGES_PATH = "stage/";
	static constexpr auto FILE_EXT = ".yaml";

	/**
	 * @brief Creates and throws an IOException.
	 * 
	 * @param fn File which caused the exception.
	 * @param what Exception text.
	 */
	void throwIOException(const std::string& fn, const std::string& what) const;
	/**
	 * @brief Creates and throws an ParserException.
	 * 
	 * @param fn File which caused the exception.
	 * @param what Exception text.
	 */
	void throwParserException(const std::string& fn, const std::string& what) const;

	void throwParserExceptionMissingKey(const std::string& fn,
		const std::string& key);
	void throwParserExceptionInvalidKeyValue(const std::string& fn,
		const std::string& key);
public:
	YAMLStageSerializer();
	
	void save(const IStageSerializer::IdType& id) const override;
	void load(const IStageSerializer::IdType& id) override;

	std::unordered_set<IStageSerializer::IdType> getAllIds() const override;
	std::unordered_map<IStageSerializer::IdType, std::string>
	getAllTitles() const override;
};

#endif // YAMLSTAGESERIALIZER_HPP
