/**
 * @file StageState.hpp
 * @author Tomáš Ludrovan
 * @brief StageState structure
 * @version 0.1
 * @date 2024-01-26
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef STAGESTATE_HPP
#define STAGESTATE_HPP

#include <memory>
#include <string>
#include <unordered_map>

#include "types.hpp"
#include "stageeditor/Common.hpp"
#include "stageeditor/StageEditorObjects.hpp"
#include "stageserializer/IStageSerializer.hpp"

struct StageState {
	static constexpr int STAGE_WIDTH_INITIAL = 1280;
	static constexpr int STAGE_HEIGHT_INITIAL = 720;
	static constexpr std::string STAGE_TITLE_DEFAULT = "My stage";
	static constexpr int STAGE_WIDTH_MIN = 100;
	static constexpr int STAGE_HEIGHT_MIN = 100;

	std::string title;
	int width;
	int height;
	std::unordered_map<EditorOID, StageEditorObstacleObject> obstacles;
	std::unordered_map<EditorOID, StageEditorPlayerObject> players;
	IStageSerializer::IdType stageId;

	StageState()
		: title{STAGE_TITLE_DEFAULT}
		, width{STAGE_WIDTH_INITIAL}
		, height{STAGE_HEIGHT_INITIAL}
		, stageId()
	{}

	Size2d getSize() const {
		return Size2d(width, height);
	}
	bool isIdAssociated() const {
		return !stageId.empty();
	}
	void assignId(const IStageSerializer::IdType& fn) {
		stageId = fn;
	}
};

#endif // STAGESTATE_HPP
