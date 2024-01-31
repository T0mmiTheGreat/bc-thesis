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

void StageEditor::addPlayerInternal(double x, double y)
{
	EditorOID oid = generateEditorOID();

	m_stageState.players[oid] = PointF(x, y);
	m_lastAction = StageEditorAction::createActionAddPlayer(x, y, oid);
	m_history.pushAction(m_lastAction);
}

StageEditor::StageEditor()
	: m_stageState{
		.width = STAGE_WIDTH_INITIAL,
		.height = STAGE_HEIGHT_INITIAL,
	}
{}

const StageState& StageEditor::getState()
{
	return m_stageState;
}

const std::shared_ptr<StageEditorAction> StageEditor::getLastAction()
{
	return m_lastAction;
}

void StageEditor::addPlayer(double x, double y, ObjectSnap snapping)
{
	double xSnap, ySnap;
	getSnappedXY(x, y, snapping, xSnap, ySnap);
	addPlayerInternal(xSnap, ySnap);
}

void StageEditor::addPlayer(const PointF& pos, ObjectSnap snapping)
{
	addPlayer(pos.x, pos.y, snapping);
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
