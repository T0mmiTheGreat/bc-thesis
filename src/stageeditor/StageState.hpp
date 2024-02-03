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

#include <unordered_map>

#include "types.hpp"
#include "stageeditor/Common.hpp"
#include "stageeditor/StageEditorObjects.hpp"

struct StageState {
	int width;
	int height;
	std::unordered_map<EditorOID, StageEditorObstacleObject> obstacles;
	std::unordered_map<EditorOID, StageEditorPlayerObject> players;

	Size2d getSize() const {
		return Size2d(width, height);
	}
};

#endif // STAGESTATE_HPP
