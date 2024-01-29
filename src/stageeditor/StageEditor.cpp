/**
 * @file StageEditor.cpp
 * @author Tomáš Ludrovan
 * @brief StageEditor class
 * @version 0.1
 * @date 2024-01-26
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "stageeditor/StageEditor.hpp"

#include "math/Math.hpp"
#include "stageeditor/Common.hpp"

void StageEditor::getSnappedXY(double x, double y, ObjectSnap snapping,
	double& xSnap, double& ySnap)
{
	switch (snapping) {
		case SNAP_NONE:
			xSnap = x;
			ySnap = y;
			break;
		case SNAP_STEP1:
			xSnap = roundToMultiple(x, 1.0);
			ySnap = roundToMultiple(y, 1.0);
			break;
		case SNAP_STEP10:
			xSnap = roundToMultiple(x, 10.0);
			ySnap = roundToMultiple(y, 10.0);
			break;
		case SNAP_STEP100:
			xSnap = roundToMultiple(x, 100.0);
			ySnap = roundToMultiple(y, 100.0);
			break;
	}
}

void StageEditor::toolSelectClick(double x, double y, ObjectSnap snapping)
{
	// TODO
}

void StageEditor::toolPlayersClick(double x, double y, ObjectSnap snapping)
{
	double xSnap, ySnap;
	getSnappedXY(x, y, snapping, xSnap, ySnap);
	addPlayer(xSnap, ySnap);
}

void StageEditor::toolObstaclesClick(double x, double y, ObjectSnap snapping)
{
	double xSnap, ySnap;
	getSnappedXY(xSnap, ySnap, snapping, xSnap, ySnap);
	// TODO
}

void StageEditor::addPlayer(double x, double y)
{
	EditorOID oid = generateEditorOID();

	m_stageState.players[oid] = PointF(x, y);
	m_lastAction = StageEditorAction::createActionAddPlayer(x, y, oid);
	m_history.pushAction(m_lastAction);
}

StageEditor::StageEditor()
	: m_activeTool{TOOL_SELECT}
	, m_stageState{
		.width = STAGE_WIDTH_INITIAL,
		.height = STAGE_HEIGHT_INITIAL,
	}
{}

const StageState& StageEditor::getState()
{
	return m_stageState;
}

StageEditor::Tool StageEditor::getActiveTool()
{
	return m_activeTool;
}

void StageEditor::setActiveTool(StageEditor::Tool tool)
{
	m_activeTool = tool;
}

const std::shared_ptr<StageEditorAction> StageEditor::getLastAction()
{
	return m_lastAction;
}

void StageEditor::click(double x, double y, StageEditor::ObjectSnap snapping)
{
	switch (m_activeTool) {
		case TOOL_SELECT:
			toolSelectClick(x, y, snapping);
			break;
		case TOOL_PLAYERS:
			toolPlayersClick(x, y, snapping);
			break;
		case TOOL_OBSTACLES:
			toolObstaclesClick(x, y, snapping);
			break;
	}
}

void StageEditor::undo()
{
	// TODO
}

void StageEditor::redo()
{
	// TODO
}

bool StageEditor::canUndo()
{
	return m_history.canUndo();
}

bool StageEditor::canRedo()
{
	return m_history.canRedo();
}
