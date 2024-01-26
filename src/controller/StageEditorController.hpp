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

#include "types.hpp"
#include "canvas/ICanvas.hpp"
#include "controller/GeneralControllerBase.hpp"
#include "sprite/EditorIconSprite.hpp"
#include "sprite/HorizontalLineSprite.hpp"
#include "sprite/VerticalLineSprite.hpp"
#include "sprite/StatusbarTextSprite.hpp"

class StageEditorController : public GeneralControllerBase {
private:
	static constexpr int MENUICON_NEW_IDX = 0;
	static constexpr int MENUICON_OPEN_IDX = MENUICON_NEW_IDX + 1;
	static constexpr int MENUICON_SAVE_IDX = MENUICON_OPEN_IDX + 1;
	static constexpr int MENUICON_SAVE_AS_IDX = MENUICON_SAVE_IDX + 1;
	static constexpr int MENUICON_UNDO_IDX = MENUICON_SAVE_AS_IDX + 1;
	static constexpr int MENUICON_REDO_IDX = MENUICON_UNDO_IDX + 1;
	static constexpr int MENUICON_COUNT = MENUICON_REDO_IDX + 1;

	static constexpr int MENUICONS_WIDTH = 32;
	static constexpr int MENUICONS_HEIGHT = 32;
	static constexpr int MENUICONS_TOP_MARGIN = 8;
	static constexpr int MENUICONS_LEFT_MARGIN = 8;
	static constexpr int MENUICONS_BOTTOM_MARGIN = 8;
	static constexpr int MENUICONS_SPACING = 6;

	static constexpr int MENUBAR_HEIGHT = MENUICONS_TOP_MARGIN
		+ MENUICONS_HEIGHT + MENUICONS_BOTTOM_MARGIN;
	
	static constexpr int TOOLICON_SELECT_IDX = 0;
	static constexpr int TOOLICON_PLAYER_IDX = TOOLICON_SELECT_IDX + 1;
	static constexpr int TOOLICON_OBSTACLE_IDX = TOOLICON_PLAYER_IDX + 1;
	static constexpr int TOOLICON_COUNT = TOOLICON_OBSTACLE_IDX + 1;

	static constexpr int TOOLICONS_WIDTH = MENUICONS_WIDTH;
	static constexpr int TOOLICONS_HEIGHT = MENUICONS_HEIGHT;
	static constexpr int TOOLICONS_TOP_MARGIN = MENUICONS_TOP_MARGIN;
	static constexpr int TOOLICONS_LEFT_MARGIN = MENUICONS_LEFT_MARGIN;
	static constexpr int TOOLICONS_RIGHT_MARGIN = TOOLICONS_LEFT_MARGIN;
	static constexpr int TOOLICONS_HORZ_SPACING = MENUICONS_SPACING;
	static constexpr int TOOLICONS_VERT_SPACING = TOOLICONS_HORZ_SPACING;

	static constexpr int TOOLBAR_Y = MENUBAR_HEIGHT;
	static constexpr int TOOLBAR_ITEM_COLUMNS = 3;
	static constexpr int TOOLBAR_WIDTH = TOOLICONS_LEFT_MARGIN
		+ (TOOLICONS_WIDTH * TOOLBAR_ITEM_COLUMNS)
		+ (TOOLICONS_HORZ_SPACING * (TOOLBAR_ITEM_COLUMNS - 1))
		+ TOOLICONS_RIGHT_MARGIN;
	
	static constexpr int STATUSBAR_HEIGHT = 30;
	static constexpr int STATUSBAR_TEXT_LEFT_MARGIN = 8;
	static constexpr FontId STATUSBAR_TEXT_FONT = FONT_TAHOMA_16;

	std::array<std::unique_ptr<EditorIconSprite>, MENUICON_COUNT> m_menuIcons;
	std::unique_ptr<HorizontalLineSprite> m_menuBarLine;
	std::array<std::unique_ptr<EditorIconSprite>, TOOLICON_COUNT> m_toolIcons;
	std::unique_ptr<VerticalLineSprite> m_toolBarLine;
	std::unique_ptr<StatusbarTextSprite> m_statusBarText;
	std::unique_ptr<HorizontalLineSprite> m_statusBarLine;

	void createSprites();
	void positionSprites();

	Rect getMenubarRect();
	Rect getToolbarRect();
	Rect getStatusbarRect();
	Rect getDesktopRect();
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
	 * @brief Mouse moved above the desktop.
	 */
	void mouseMoveDesktop(int x, int y);
public:
	StageEditorController(std::shared_ptr<ISysProxy> sysProxy);
	void startedEvent() override;
	void mouseMoveEvent(int x, int y) override;
	void paintEvent(std::shared_ptr<ICanvas> canvas, Rect& invalidRect) override;
};

#endif // STAGEEDITORCONTROLLER_HPP
