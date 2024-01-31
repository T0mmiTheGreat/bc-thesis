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

void StageEditor::getSnappedPoint(const PointF& p, ObjectSnap snapping,
	PointF& pSnap)
{
	switch (snapping) {
		case SNAP_NONE:
			pSnap = p;
			break;
		case SNAP_STEP1:
			pSnap.x = roundToMultiple(p.x, 1.0);
			pSnap.y = roundToMultiple(p.y, 1.0);
			break;
		case SNAP_STEP10:
			pSnap.x = roundToMultiple(p.x, 10.0);
			pSnap.y = roundToMultiple(p.y, 10.0);
			break;
		case SNAP_STEP100:
			pSnap.x = roundToMultiple(p.x, 100.0);
			pSnap.y = roundToMultiple(p.y, 100.0);
			break;
	}
}

void StageEditor::addPlayerInternal(const PointF& pos)
{
	EditorOID oid = generateEditorOID();

	m_stageState.players[oid] = pos;
	m_lastAction = std::make_shared<StageEditorActionAddPlayer>(pos, oid);
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
	addPlayer(PointF(x, y), snapping);
}

void StageEditor::addPlayer(const PointF& pos, ObjectSnap snapping)
{
	PointF posSnap;
	getSnappedPoint(pos, snapping, posSnap);
	addPlayerInternal(posSnap);
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
