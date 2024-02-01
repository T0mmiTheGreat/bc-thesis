/**
 * @file StageEditorController.hpp
 * @author Tomáš Ludrovan
 * @brief StageEditorController class
 * @version 0.1
 * @date 2024-01-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef STAGEEDITORCONTROLLER_HPP
#define STAGEEDITORCONTROLLER_HPP

#include <array>
#include <memory>
#include <unordered_map>

#include "types.hpp"
#include "canvas/ICanvas.hpp"
#include "controller/GeneralControllerBase.hpp"
#include "sprite/EditorIconSprite.hpp"
#include "sprite/OptionBarSprite.hpp"
#include "sprite/StatusbarTextSprite.hpp"
#include "sprite/EditorWorkspaceGridSprite.hpp"
#include "sprite/PlayerSprite.hpp"
#include "sprite/ObstacleSprite.hpp"
#include "sprite/ObstacleEdgesSprite.hpp"
#include "stageeditor/Common.hpp"
#include "stageeditor/StageEditor.hpp"
#include "stageeditor/StageViewport.hpp"

class StageEditorController : public GeneralControllerBase {
private:
	enum EditorTool {
		TOOL_SELECT = 0,
		TOOL_PLAYERS,
		TOOL_OBSTACLES,
	};
private:
#pragma region constants
	static constexpr int MENUICON_NEW_IDX = 0;
	static constexpr int MENUICON_OPEN_IDX = MENUICON_NEW_IDX + 1;
	static constexpr int MENUICON_SAVE_IDX = MENUICON_OPEN_IDX + 1;
	static constexpr int MENUICON_SAVE_AS_IDX = MENUICON_SAVE_IDX + 1;
	static constexpr int MENUICON_UNDO_IDX = MENUICON_SAVE_AS_IDX + 1;
	static constexpr int MENUICON_REDO_IDX = MENUICON_UNDO_IDX + 1;
	static constexpr int MENUICON_COUNT = MENUICON_REDO_IDX + 1;

	static constexpr int MENUICONS_WIDTH = EditorIconSprite::FIXED_WIDTH;
	static constexpr int MENUICONS_HEIGHT = EditorIconSprite::FIXED_HEIGHT;
	static constexpr int MENUICONS_TOP_MARGIN = 8;
	static constexpr int MENUICONS_LEFT_MARGIN = 8;
	static constexpr int MENUICONS_BOTTOM_MARGIN = 8;
	static constexpr int MENUICONS_SPACING = 6;

	static constexpr int MENUBAR_HEIGHT = MENUICONS_TOP_MARGIN
		+ MENUICONS_HEIGHT + MENUICONS_BOTTOM_MARGIN;
	static constexpr Color MENUBAR_FCOLOR = Color::black();
	static constexpr Color MENUBAR_SCOLOR = Color::white();
	
	static constexpr int TOOLICON_SELECT_IDX = 0;
	static constexpr int TOOLICON_PLAYER_IDX = TOOLICON_SELECT_IDX + 1;
	static constexpr int TOOLICON_OBSTACLE_IDX = TOOLICON_PLAYER_IDX + 1;
	static constexpr int TOOLICON_COUNT = TOOLICON_OBSTACLE_IDX + 1;

	static constexpr int TOOLICONS_WIDTH = EditorIconSprite::FIXED_WIDTH;
	static constexpr int TOOLICONS_HEIGHT = EditorIconSprite::FIXED_HEIGHT;
	static constexpr int TOOLICONS_TOP_MARGIN = MENUICONS_TOP_MARGIN;
	static constexpr int TOOLICONS_LEFT_MARGIN = MENUICONS_LEFT_MARGIN;
	static constexpr int TOOLICONS_RIGHT_MARGIN = TOOLICONS_LEFT_MARGIN;
	static constexpr int TOOLICONS_HORZ_SPACING = MENUICONS_SPACING;
	static constexpr int TOOLICONS_VERT_SPACING = TOOLICONS_HORZ_SPACING;

	static constexpr int TOOLBAR_Y = MENUBAR_HEIGHT + 1;
	static constexpr int TOOLBAR_ITEM_COLUMNS = 3;
	static constexpr int TOOLBAR_WIDTH = TOOLICONS_LEFT_MARGIN
		+ (TOOLICONS_WIDTH * TOOLBAR_ITEM_COLUMNS)
		+ (TOOLICONS_HORZ_SPACING * (TOOLBAR_ITEM_COLUMNS - 1))
		+ TOOLICONS_RIGHT_MARGIN;
	static constexpr Color TOOLBAR_FCOLOR = MENUBAR_FCOLOR;
	static constexpr Color TOOLBAR_SCOLOR = MENUBAR_SCOLOR;
	
	static constexpr int STATUSBAR_HEIGHT = 30;
	static constexpr int STATUSBAR_TEXT_LEFT_MARGIN = 8;
	static constexpr FontId STATUSBAR_TEXT_FONT = FONT_TAHOMA_16;
	static constexpr Color STATUSBAR_FCOLOR = MENUBAR_FCOLOR;
	static constexpr Color STATUSBAR_SCOLOR = MENUBAR_SCOLOR;
#pragma endregion constants

	EditorTool m_activeTool;

	// XXX: Keep the editor and viewport in this order for correct order of
	//      initialization

	StageEditor m_stageEditor;
	StageViewport m_viewport;

#pragma region sprites
	std::array<std::unique_ptr<EditorIconSprite>, MENUICON_COUNT> m_menuIcons;
	std::unique_ptr<OptionBarSprite> m_menuBarSprite;
	std::array<std::unique_ptr<EditorIconSprite>, TOOLICON_COUNT> m_toolIcons;
	std::unique_ptr<OptionBarSprite> m_toolBarSprite;
	std::unique_ptr<StatusbarTextSprite> m_statusBarText;
	std::unique_ptr<OptionBarSprite> m_statusBarSprite;

	std::unique_ptr<EditorWorkspaceGridSprite> m_gridSprite;
	std::unordered_map<EditorOID, std::unique_ptr<PlayerSprite>> m_playerSprites;
	std::unordered_map<EditorOID, std::unique_ptr<ObstacleSprite>> m_obstacleSprites;

	// Will be created and destroyed repeatedly. Checking for `nullptr` should
	// be done.
	std::unique_ptr<ObstacleEdgesSprite> m_obstacleEdges;
#pragma endregion sprites

	void createSprites();
	void positionSprites();

	Rect getMenubarRect();
	Rect getToolbarRect();
	Rect getStatusbarRect();
	Rect getWorkspaceRect();
	Rect getMenuIconRect(int iconIdx);
	Rect getToolIconRect(int iconIdx);

	/**
	 * @brief If the icon has COSTUME_HOVER changes it to COSTUME_NORMAL.
	 */
	void iconHighlightOff(std::unique_ptr<EditorIconSprite>& icon);
	/**
	 * @brief If the icon has COSTUME_NORMAL changes it to COSTUME_HOVER.
	 */
	void iconHighlightOn(std::unique_ptr<EditorIconSprite>& icon);
	/**
	 * @brief Finds all icons with COSTUME_HOVER and changes it to
	 *        COSTUME_NORMAL.
	 */
	void iconHighlightOffAll();

	/**
	 * @brief Mouse moved above the menu bar.
	 */
	void mouseMoveMenubar(int x, int y);
	/**
	 * @brief Mouse moved above the tool bar.
	 */
	void mouseMoveToolbar(int x, int y);
	/**
	 * @brief Mouse moved above the status bar.
	 */
	void mouseMoveStatusbar(int x, int y);
	/**
	 * @brief Mouse moved above the workspace.
	 */
	void mouseMoveWorkspace(int x, int y);

	/**
	 * @brief Mouse button pressed inside menu bar.
	 */
	void mouseBtnDownMenubar(MouseBtn btn, int x, int y);
	/**
	 * @brief Mouse button pressed inside tool bar.
	 */
	void mouseBtnDownToolbar(MouseBtn btn, int x, int y);
	/**
	 * @brief Mouse button pressed inside status bar.
	 */
	void mouseBtnDownStatusbar(MouseBtn btn, int x, int y);
	/**
	 * @brief Mouse button pressed inside workspace.
	 */
	void mouseBtnDownWorkspace(MouseBtn btn, int x, int y);
	/**
	 * @brief Mouse button pressed inside workspace with the active tool being
	 *        the "select tool".
	 */
	void mouseBtnDownWorkspaceToolSelect(MouseBtn btn, int x, int y);
	/**
	 * @brief Mouse button pressed inside workspace with the active tool being
	 *        the "players tool".
	 */
	void mouseBtnDownWorkspaceToolPlayers(MouseBtn btn, int x, int y);
	/**
	 * @brief Mouse button pressed inside workspace with the active tool being
	 *        the "obstacles tool".
	 */
	void mouseBtnDownWorkspaceToolObstacles(MouseBtn btn, int x, int y);

	/**
	 * @brief Updates all sprites after a change in backend (StageEditor).
	 */
	void updateSpritesByBackend();
	/**
	 * @brief Updates all sprites after a change in viewport.
	 */
	void updateSpritesByViewport();
	/**
	 * @brief Updates player sprite after a change in backend or viewport.
	 * 
	 * @param oid Editor OID of the player object.
	 * 
	 * @remark This method does the same thing as the overloaded variant below,
	 *         but it has to calculate the other parameters itself. It is
	 *         convenient when only one player sprite is updated, but for
	 *         multiple sprites, the one below should be preferred.
	 */
	void updatePlayerSprite(EditorOID oid);
	/**
	 * @brief Updates player sprite after a change in backend or viewport.
	 * 
	 * @param oid Editor OID of the player object.
	 * @param sprite The sprite associated with the OID.
	 * @param tm Transformation matrix to apply to the player object position.
	 *           The method will make the sprite position adjustments itself.
	 *           Caller should just pass the result of `StageViewport::
	 *           getProjectionMatrix()`.
	 * @param radius Radius of the sprite. This should be calculated as
	 *               `EDITOR_PLAYER_RADIUS * StageViewport::getZoom()`.
	 * 
	 * @remark This methods should be used in "for each player loops" to save
	 *         computation time (the parameters will be the same for each
	 *         sprite).
	 */
	void updatePlayerSprite(EditorOID oid,
		std::unique_ptr<PlayerSprite>& sprite, const Matrix3x3& tm,
		double radius);
	void updateObstacleEdgesSprite();
	void updateObstacleEdgesSprite(const Matrix3x3& tm);
	void updateObstacleSprite(EditorOID oid);

	void addPlayerSprite(const PointF& pos, EditorOID oid);
	void addObstacleEdge(const PointF& p);
	void addObstacleSprite(EditorOID oid);
public:
	StageEditorController(std::shared_ptr<ISysProxy> sysProxy);
	void startedEvent() override;
	void mouseBtnDownEvent(MouseBtn btn, int x, int y) override;
	void mouseBtnUpEvent(MouseBtn btn, int x, int y) override;
	void mouseMoveEvent(int x, int y) override;
	void mouseWheelEvent(int dx, int dy) override;
	void paintEvent(std::shared_ptr<ICanvas> canvas, Rect& invalidRect) override;
};

#endif // STAGEEDITORCONTROLLER_HPP
