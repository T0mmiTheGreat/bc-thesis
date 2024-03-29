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
#include "sprite/PlayerBrushSprite.hpp"
#include "sprite/ObstacleBrushSprite.hpp"
#include "stageeditor/Common.hpp"
#include "stageeditor/StageEditor.hpp"
#include "stageviewport/StageViewport.hpp"

class StageEditorController : public GeneralControllerBase {
private:
	enum ExitResult {
		RES_MENU,
		RES_STAGE_PROPERTIES,
		RES_STAGE_SELECT,
	};
private:
#pragma region constants
	static constexpr int MENUICON_NEW_IDX        = 0;
	static constexpr int MENUICON_OPEN_IDX       = MENUICON_NEW_IDX + 1;
	static constexpr int MENUICON_SAVE_IDX       = MENUICON_OPEN_IDX + 1;
	static constexpr int MENUICON_UNDO_IDX       = MENUICON_SAVE_IDX + 1;
	static constexpr int MENUICON_REDO_IDX       = MENUICON_UNDO_IDX + 1;
	static constexpr int MENUICON_PROPERTIES_IDX = MENUICON_REDO_IDX + 1;
	static constexpr int MENUICON_BACK_IDX       = MENUICON_PROPERTIES_IDX + 1;
	static constexpr int MENUICON_COUNT = MENUICON_BACK_IDX + 1;

	static constexpr int MENUICONS_WIDTH = EditorIconSprite::FIXED_WIDTH;
	static constexpr int MENUICONS_HEIGHT = EditorIconSprite::FIXED_HEIGHT;
	static constexpr int MENUICONS_TOP_MARGIN = 8;
	static constexpr int MENUICONS_LEFT_MARGIN = 8;
	static constexpr int MENUICONS_BOTTOM_MARGIN = 8;
	static constexpr int MENUICONS_RIGHT_MARGIN = MENUICONS_LEFT_MARGIN;
	static constexpr int MENUICONS_SPACING = 6;

	static constexpr int MENUBAR_HEIGHT = MENUICONS_TOP_MARGIN
		+ MENUICONS_HEIGHT + MENUICONS_BOTTOM_MARGIN;
	static constexpr Color MENUBAR_FCOLOR = Color::black();
	static constexpr Color MENUBAR_SCOLOR = Color::white();
	
	static constexpr int TOOLICON_SELECT_IDX    = 0;
	static constexpr int TOOLICON_PLAYER_IDX    = TOOLICON_SELECT_IDX + 1;
	static constexpr int TOOLICON_OBSTACLE_IDX  = TOOLICON_PLAYER_IDX + 1;
	static constexpr int TOOLICON_TRASH_CAN_IDX = TOOLICON_OBSTACLE_IDX + 1;
	static constexpr int TOOLICON_RESIZE_IDX    = TOOLICON_TRASH_CAN_IDX + 1;
	static constexpr int TOOLICON_COUNT = TOOLICON_RESIZE_IDX + 1;

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
	static constexpr FontId STATUSBAR_TEXT_FONT = FONT_ROBOTO_16;
	static constexpr Color STATUSBAR_FCOLOR = MENUBAR_FCOLOR;
	static constexpr Color STATUSBAR_SCOLOR = MENUBAR_SCOLOR;

	// How many pixels the viewport moves when the mouse wheel is scrolled
	static constexpr double VIEWPORT_MOVE_PER_SCROLL = 32.0;
#pragma endregion constants

	// XXX: Keep the editor and viewport in this order for correct order of
	//      initialization

	std::shared_ptr<StageEditor> m_stageEditor;
	StageViewport m_viewport;

	ExitResult m_exitResult;

	// Used when selecting a stage to load (passed as parameters to
	// StageSelectController constructor).

	IStageSerializer::IdType m_selectedStage;
	bool m_isSelectedStageValid;

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

	std::unique_ptr<ObstacleEdgesSprite> m_obstacleEdges;

	std::unique_ptr<PlayerBrushSprite> m_playerBrush;
	std::unique_ptr<ObstacleBrushSprite> m_obstacleBrush;
	// Either `m_playerBrush`, `m_obstacleBrush`, or nullptr
	ISprite* m_brushSprite;
	
	std::unordered_map<EditorOID, std::unique_ptr<PlayerSprite>> m_draggedPlayerSprites;
	std::unordered_map<EditorOID, std::unique_ptr<ObstacleSprite>> m_draggedObstacleSprites;
#pragma endregion sprites

	void createSprites();
	void initializeSprites();
	void positionSprites();

	/**
	 * @brief Mouse moved above the menu bar.
	 */
	void mouseMoveMenubar(int x, int y);
	/**
	 * @brief Mouse moved above the tool bar.
	 */
	void mouseMoveToolbar(int x, int y);
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
	 * @brief Mouse button pressed inside workspace.
	 */
	void mouseBtnDownWorkspace(MouseBtn btn, int x, int y);

	/**
	 * @brief Mouse scrolled over workspace.
	 * 
	 * @param dx The amount scrolled horizontally.
	 * @param dy The amount scrolled vertically.
	 */
	void mouseWheelWorkspace(int dx, int dy);

	void checkMenuIconMouseHover(int x, int y);
	void checkToolIconMouseHover(int x, int y);
	void checkWorkspaceDoDrag(int x, int y);

	void checkMenuIconClick(int x, int y);
	void checkToolIconClick(int x, int y);

	void menuIconClick(int iconIdx);
	void menuIconNewClick();
	void menuIconOpenClick();
	void menuIconSaveClick();
	void menuIconUndoClick();
	void menuIconRedoClick();
	void menuIconPropertiesClick();
	void menuIconBackClick();

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
	 * @brief Change the appearance of a tool icon sprite to "selected".
	 * 
	 * @param iconIdx Index of the tool icon.
	 */
	void toolIconSetSelected(int iconIdx);
	/**
	 * @brief Change the appearance of a tool icon sprite to "not selected".
	 * 
	 * @param iconIdx Index of the tool icon.
	 */
	void toolIconUnsetSelected(int iconIdx);
	/**
	 * @brief Sets the enabled state (enabled/disabled) of a menu icon.
	 * 
	 * @param iconIdx 
	 * @param value `true` for "enabled", `false` for "disabled".
	 */
	void menuIconSetEnabled(int iconIdx, bool value);

	/**
	 * @brief Makes the brush sprite not visible.
	 */
	void hideBrush();

	/**
	 * @brief Updates all existing sprites (does not create new ones).
	 */
	void updateAllSprites();

	/**
	 * @brief Updates all sprites after a change in viewport.
	 */
	void updateSpritesByViewport();

	/**
	 * @brief Updates all sprites after mouse movement.
	 * 
	 */
	void updateSpritesByMousePos(const Point& mouse);

	/**
	 * @brief Updates the state of undo and redo buttons.
	 */
	void updateUndoRedoIcons();

	void updateSpritesByAction(const std::shared_ptr<StageEditorAction> action);
	void updateSpritesByActionMultiple(
		const std::shared_ptr<StageEditorAction> action);
	void updateSpritesByActionAddPlayerObject(
		const std::shared_ptr<StageEditorAction> action);
	void updateSpritesByActionAddObstacleObject(
		const std::shared_ptr<StageEditorAction> action);
	void updateSpritesByActionPlaceObstacleCorner(
		const std::shared_ptr<StageEditorAction> action);
	void updateSpritesByActionUnplaceObstacleCorner(
		const std::shared_ptr<StageEditorAction> action);
	void updateSpritesByActionActivateTool(
		const std::shared_ptr<StageEditorAction> action);
	void updateSpritesByActionSelectPlayerObject(
		const std::shared_ptr<StageEditorAction> action);
	void updateSpritesByActionSelectObstacleObject(
		const std::shared_ptr<StageEditorAction> action);
	void updateSpritesByActionDeselectPlayerObject(
		const std::shared_ptr<StageEditorAction> action);
	void updateSpritesByActionDeselectObstacleObject(
		const std::shared_ptr<StageEditorAction> action);
	void updateSpritesByActionBeginDragSelected(
		const std::shared_ptr<StageEditorAction> action);
	void updateSpritesByActionMovePlayerObject(
		const std::shared_ptr<StageEditorAction> action);
	void updateSpritesByActionMoveObstacleObject(
		const std::shared_ptr<StageEditorAction> action);
	void updateSpritesByActionDeletePlayerObject(
		const std::shared_ptr<StageEditorAction> action);
	void updateSpritesByActionDeleteObstacleObject(
		const std::shared_ptr<StageEditorAction> action);
	void updateSpritesByActionBeginDragStageCorner(
		const std::shared_ptr<StageEditorAction> action);
	void updateSpritesByActionResizeStage(
		const std::shared_ptr<StageEditorAction> action);

	/**
	 * @brief Updates grid sprite after a change in backend or viewport.
	 */
	void updateGridSprite();
	/**
	 * @brief Updates player sprite after a change in backend or viewport.
	 * 
	 * @param oid Editor OID of the player object.
	 */
	void updatePlayerSprite(EditorOID oid);
	/**
	 * @brief Updates obstacle edges sprite after a change in backend or viewport.
	 */
	void updateObstacleEdgesSprite();
	/**
	 * @brief Updates obstacle sprite after a change in backend or viewport.
	 * 
	 * @param oid Editor OID of the obstacle object.
	 */
	void updateObstacleSprite(EditorOID oid);
	/**
	 * @brief Updates dragged sprites after a change in backend or viewport.
	 */
	void updateDraggedSprites();
	/**
	 * @brief Updates dragged player sprite after a change in backend or
	 *        viewport.
	 * 
	 * @param oid Editor OID of the player object.
	 */
	void updateDraggedPlayerSprite(EditorOID oid);
	/**
	 * @brief Updates dragged obstacle sprite after a change in backend or
	 *        viewport.
	 * 
	 * @param oid Editor OID of the obstacle object.
	 */
	void updateDraggedObstacleSprite(EditorOID oid);

	void updateToolBrush();
	void updateToolBrushSelect();
	void updateToolBrushPlayers();
	void updateToolBrushObstacles();
	void updateToolBrushDelete();
	void updateToolBrushResizeStage();

	void addPlayerSprite(EditorOID oid);
	void addObstacleSprite(EditorOID oid);

	/**
	 * @brief Creates matrix for conversion from screen space to stage space.
	 * 
	 * @details The objects passed to it must be relative to the screen origin,
	 *          not workspace origin.
	 */
	Matrix3x3 getScreenToStageMatrix();
	/**
	 * @brief Creates matrix for conversion from stage space to screen space.
	 * 
	 * @details The objects returned will be relative to the screen origin,
	 *          not workspace origin.
	 */
	Matrix3x3 getStageToScreenMatrix();

	Rect getMenubarRect();
	Rect getToolbarRect();
	Rect getStatusbarRect();
	Rect getWorkspaceRect();
	Rect getMenuIconRect(int iconIdx);
	Rect getToolIconRect(int iconIdx);

	static int toolToIconIdx(EditorTool tool);
	static EditorTool iconIdxToTool(int iconIdx);

	/**
	 * @brief Chooses the snapping value based on the zoom of the viewport.
	 */
	ObjectSnap getSnappingByViewportZoom();
	/**
	 * @brief Chooses the snapping value based on the zoom of the viewport.
	 * 
	 * @param solidsFrequency EditorWorkspaceGridSprite property.
	 */
	ObjectSnap getSnappingByViewportZoom(unsigned& solidsFrequency);

	/**
	 * @brief Chooses the snapping value based on the zoom of the viewport AND
	 *        the state of the `Alt` key.
	 */
	ObjectSnap getSnapping();

	/**
	 * @brief Chooses the grab zone size based on the zoom of the viewport.
	 * 
	 * @details Grab zone of a point is an area around a point, for instance
	 *          a stage corner. If a user clicks within this area it counts as
	 *          a click on that point. Generally, the area is smaller, the
	 *          bigger is the zoom value. This is to always make the area the
	 *          same size on the screen.
	 */
	double getGrabZoneSizeByViewportZoom();

	/**
	 * @brief Calculates the player sprite radius based on viewport zoom.
	 */
	int getPlayerSpriteRadius();

	/**
	 * @brief Calculates the position of the mouse in stage space.
	 */
	PointF getMouseInStageSpace();
	/**
	 * @brief Calculates the position of the mouse in stage space.
	 * 
	 * @param mouse Mouse position in screen space.
	 */
	PointF getMouseInStageSpace(const Point& mouse);
	/**
	 * @brief Calculates the position of the mouse in stage space.
	 * 
	 * @param x Mouse X position in screen space.
	 * @param y Mouse Y position in screen space.
	 */
	PointF getMouseInStageSpace(int x, int y);

	/**
	 * @brief Returns the current size of the stage, or the resized stage, if
	 *        the stage is being resized.
	 * 
	 * @param isValid Whether the resize can be applied to the stage.
	 */
	Size2d getPredictedStageSize(bool& isValid);
protected:
	std::shared_ptr<IControllerChild> createReplacement() override;
	
	void onActivated() override;
	void onStarted() override;
	void onResumed() override;
	void onMouseBtnDown(MouseBtn btn, int x, int y) override;
	void onMouseBtnUp(MouseBtn btn, int x, int y) override;
	void onMouseMove(int x, int y) override;
	void onMouseWheel(int dx, int dy) override;
	void onPaint(std::shared_ptr<ICanvas> canvas,
		const Rect& invalidRect) override;
public:
	StageEditorController(std::shared_ptr<ISysProxy> sysProxy);
};

#endif // STAGEEDITORCONTROLLER_HPP
