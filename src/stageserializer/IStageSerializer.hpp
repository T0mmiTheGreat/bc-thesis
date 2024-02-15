/**
 * @file IStageSerializer.hpp
 * @author Tomáš Ludrovan
 * @brief IStageSerializer interface
 * @version 0.1
 * @date 2024-02-13
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef ISTAGESERIALIZER_HPP
#define ISTAGESERIALIZER_HPP

#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "types.hpp"

/**
 * @brief Performs serialization and deserialization of stage data.
 * 
 * @details Provides abstract layer for peristent storage of the stages.
 *          The implementation may be arbitrary -- a single file, multiple
 *          files, SQL database, ...
 * 
 *          Every stage has its unique identifier and several properties:
 *            - title,
 *            - width,
 *            - height,
 *            - list of players,
 *            - list of obstacles,
 *            - list of positioning rules.
 *          The unique identifier is a string value consisting of alphanumeric
 *          characters and underscores. The length should be at most 30
 *          characters plus a suffix of any number of digits (the less the
 *          better), which should be used to uniquely identify stages with the
 *          same titles.
 * 
 *          The current implementation uses YAML files. There is one YAML file
 *          for each stage. Unique identifier is the file name.
 */
class IStageSerializer {
public:
	typedef std::string IdType;
	typedef PointF PlayerType;
	typedef TriangleF ObstacleType;
	// These are indexes to the `players` vector
	typedef std::vector<int> PositionRuleType;

	class Exception : public std::runtime_error {
	public:
		Exception(const std::string& message)
			: std::runtime_error(message)
		{}
	};
	class IOException : public Exception {
	public:
		IOException(const std::string& message)
			: Exception(message)
		{}
	};
	class ParserException : public Exception {
	public:
		ParserException(const std::string& message)
			: Exception(message)
		{}
	};

	virtual ~IStageSerializer() {}

	/**
	 * @brief Saves the stage to a persistent data storage.
	 * 
	 * @param id The stage identifier.
	 */
	virtual void save(const IStageSerializer::IdType& id) const = 0;
	/**
	 * @brief Loads the stage from a persistent data storage.
	 * 
	 * @param id The stage identifier.
	 */
	virtual void load(const IStageSerializer::IdType& id) = 0;

	// TODO: This is basically loading the whole database. A handy iterator
	//       would be better for these two functions...
	/**
	 * @brief Returns identifiers of all existing stages.
	 */
	virtual std::unordered_set<IStageSerializer::IdType> getAllIds() const = 0;
	/**
	 * @brief Returns titles of all existing stages.
	 * 
	 * @return Map of stage titles indexed using their IDs.
	 */
	virtual std::unordered_map<IStageSerializer::IdType, std::string>
	getAllTitles() const = 0;

	/**
	 * @brief Generates a stage ID based on the `title`.
	 */
	virtual IStageSerializer::IdType generateIdByTitle(
		const std::string& title) const = 0;
	/**
	 * @brief Returns the "canonical" ID for the provided `stageId`.
	 * 
	 * @details "Canonical" ID is used for identifying the stage externally. If
	 *          the stage is stored in a file, returns the path (probably
	 *          relative) to the file. For other means of storing the stage, the
	 *          value may be anything; the `stageId` itself, empty string,
	 *          hash, etc. In general, though, it would make sense to refer to
	 *          is as the "file name".
	 */
	virtual std::string getStageCanonicalId(
		const IStageSerializer::IdType& stageId) const = 0;

	virtual const std::string& getTitle() const = 0;
	virtual void setTitle(const std::string& value) = 0;
	virtual int getWidth() const = 0;
	virtual void setWidth(int value) = 0;
	virtual int getHeight() const = 0;
	virtual void setHeight(int value) = 0;
	virtual const std::vector<IStageSerializer::PlayerType>&
	getPlayers() const = 0;
	virtual void setPlayers(
		const std::vector<IStageSerializer::PlayerType>& value) = 0;
	virtual const std::vector<IStageSerializer::ObstacleType>&
	getObstacles() const = 0;
	virtual void setObstacles(
		const std::vector<IStageSerializer::ObstacleType>& value) = 0;
	virtual const std::vector<IStageSerializer::PositionRuleType>&
	getPositionRules() const = 0;
	virtual void setPositionRules(
		const std::vector<IStageSerializer::PositionRuleType>& value) = 0;
};

#endif // ISTAGESERIALIZER_HPP
