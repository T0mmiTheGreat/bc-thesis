/**
 * @file Common.hpp
 * @author Tomáš Ludrovan
 * @brief Common definitions
 * @version 0.1
 * @date 2024-01-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef COMMON_HPP
#define COMMON_HPP

/*
	Naming:
	+-------------------------------------------------+
	| menubar                                         |
	+---+---------------------------------------------+
	| t |                                             |
	| o |                                             |
	| o |                                             |
	| l |                 workspace                   |
	| b |                                             |
	| a |                                             |
	| r |                                             |
	+---+---------------------------------------------+
	| statusbar                                       |
	+-------------------------------------------------+

	- The coordinate system of the screen shall be referred to as "screen
	  space".
	- The coordinate system where the stage objects are stored shall be
	  referred to as "stage space".

	- "Projection to workspace" is conversion from stage space to screen space
	  with the screen coordinate system origin being the top left corner of
	  the workspace.
	- "Projection to screen space" is conversion from stage space to screen
	  space with the screen coordinate system origin being the top left corner
	  of the screen.
	- "Projection to stage space" is conversion from screen space to stage
	  space. Unless explicitly stated, the screen coordinate system origin shall
	  be the top left corner of the workspace.

	- "Stage position" is the position of the stage grid within workspace in the
	  screen space coordinates.
*/

/**
 * @brief Editor object ID.
 * 
 * @details This type is used for identifying editor objects -- players and
 *          obstacles. Each object shall have its own unique OID generated by
 *          generateEditorOID() function. A special value of EDITOR_OID_NULL
 *          shall be used in the manner of the `NULL` pointer in C and C++.
 */
typedef size_t EditorOID;

enum EditorTool {
	TOOL_SELECT = 0,
	TOOL_PLAYERS,
	TOOL_OBSTACLES,
	TOOL_DELETE,
};


// Number of units taken by half of a player sprite in the editor
constexpr double EDITOR_PLAYER_RADIUS = 50.0;

constexpr EditorOID EDITOR_OID_NULL = 0;


/**
 * @brief Generates a unique editor object identifier.
 */
inline EditorOID generateEditorOID()
{
	static EditorOID lastOID = 0;
	lastOID++;
	return lastOID;
}

#endif // COMMON_HPP
