/**
 * @file StageSerializerBase.hpp
 * @author Tomáš Ludrovan
 * @brief StageSerializerBase class
 * @version 0.1
 * @date 2024-02-13
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef STAGESERIALIZERBASE_HPP
#define STAGESERIALIZERBASE_HPP

#include "stageserializer/IStageSerializer.hpp"

class StageSerializerBase : public IStageSerializer {
protected:
	std::string title;
	int width;
	int height;
	std::vector<PlayerType> players;
	std::vector<ObstacleType> obstacles;
	std::vector<PositionRuleType> positionRules;
public:
	StageSerializerBase();
	/**
	 * @brief Saves the stage to a persistent data storage.
	 * 
	 * @param id The stage identifier.
	 */
	virtual void save(const IStageSerializer::IdType& id) const override = 0;
	/**
	 * @brief Loads the stage from a persistent data storage.
	 * 
	 * @param id The stage identifier.
	 */
	virtual void load(const IStageSerializer::IdType& id) override = 0;

	/**
	 * @brief Returns identifiers of all existing stages.
	 */
	virtual std::unordered_set<IStageSerializer::IdType>
	getAllIds() const override = 0;
	/**
	 * @brief Returns titles of all existing stages.
	 * 
	 * @return Map of stage titles indexed using their IDs.
	 */
	virtual std::unordered_map<IStageSerializer::IdType, std::string>
	getAllTitles() override;

	/**
	 * @brief Generates a stage ID based on the `title`.
	 */
	virtual IStageSerializer::IdType generateIdByTitle(
		const std::string& title) const override;

	const std::string& getTitle() const override;
	void setTitle(const std::string& value) override;
	int getWidth() const override;
	void setWidth(int value) override;
	int getHeight() const override;
	void setHeight(int value) override;
	const std::vector<IStageSerializer::PlayerType>&
	getPlayers() const override;
	void setPlayers(
		const std::vector<IStageSerializer::PlayerType>& value) override;
	const std::vector<IStageSerializer::ObstacleType>&
	getObstacles() const override;
	void setObstacles(
		const std::vector<IStageSerializer::ObstacleType>& value) override;
	const std::vector<IStageSerializer::PositionRuleType>&
	getPositionRules() const override;
	void setPositionRules(
		const std::vector<IStageSerializer::PositionRuleType>& value) override;
};

#endif // STAGESERIALIZERBASE_HPP
