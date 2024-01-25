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
#include "sprite/ImageSprite.hpp"
#include "sprite/HorizontalLineSprite.hpp"

class StageEditorController : public GeneralControllerBase {
private:
	static constexpr int MENUICON_NEW_IDX = 0;
	static constexpr int MENUICON_OPEN_IDX = MENUICON_NEW_IDX + 1;
	static constexpr int MENUICON_SAVE_IDX = MENUICON_OPEN_IDX + 1;
	static constexpr int MENUICON_SAVE_AS_IDX = MENUICON_SAVE_IDX + 1;
	static constexpr int MENUICON_COUNT = MENUICON_SAVE_AS_IDX + 1;

	static constexpr int MENUICONS_WIDTH = 32;
	static constexpr int MENUICONS_HEIGHT = 32;
	static constexpr int MENUICONS_TOP_MARGIN = 8;
	static constexpr int MENUICONS_LEFT_MARGIN = 8;
	static constexpr int MENUICONS_BOTTOM_MARGIN = 8;
	static constexpr int MENUICONS_SPACING = 6;

	static constexpr int MENUBAR_HEIGHT = MENUICONS_TOP_MARGIN
		+ MENUICONS_HEIGHT + MENUICONS_BOTTOM_MARGIN;

	std::array<std::unique_ptr<ImageSprite>, MENUICON_COUNT> m_icons;
	std::unique_ptr<HorizontalLineSprite> m_menuBarLine;

	void createSprites();
public:
	StageEditorController(std::shared_ptr<ISysProxy> sysProxy);
	void startedEvent() override;
	void paintEvent(std::shared_ptr<ICanvas> canvas, Rect& invalidRect) override;
};

#endif // STAGEEDITORCONTROLLER_HPP
