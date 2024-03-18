/**
 * @file StagePropertiesController.hpp
 * @author Tomáš Ludrovan
 * @brief StagePropertiesController class
 * @version 0.1
 * @date 2024-02-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef STAGEPROPERTIESCONTROLLER_HPP
#define STAGEPROPERTIESCONTROLLER_HPP

#include <memory>
#include <string>

#include "controller/GeneralControllerBase.hpp"
#include "sysproxy/ISysProxy.hpp"
#include "sprite/TextSprite.hpp"
#include "sprite/TextBoxSprite.hpp"
#include "sprite/OptionButtonSprite.hpp"
#include "stageeditor/StageEditor.hpp"

class StagePropertiesController : public GeneralControllerBase {
private:
	static constexpr int TEXTBOX_TOP = 220;
	static constexpr int TEXTBOX_WIDTH = 220;
	static constexpr int BUTTONS_TOP = 272;
	static constexpr int BUTTONS_SPACING = 8;

	static constexpr const char* OK_BTN_TEXT = "OK";
	static constexpr const char* CANCEL_BTN_TEXT = "Cancel";

	static constexpr FontId TEXTBOX_FONT = FONT_TAHOMA_20;
	static constexpr FontId FILENAME_FONT = FONT_TAHOMA_16;
	static constexpr Color TEXTBOX_TEXT_COLOR = Color::white();
	static constexpr Color FILENAME_TEXT_COLOR = Color::white().setAlpha(0x88);

	std::shared_ptr<StageEditor> m_stageEditor;
	std::string m_stageTitle;
	std::unique_ptr<TextBoxSprite> m_textBoxSprite;
	std::unique_ptr<TextSprite> m_filenameSprite;
	std::unique_ptr<OptionButtonSprite> m_okBtnSprite;
	std::unique_ptr<OptionButtonSprite> m_cancelBtnSprite;

	Rect getOkBtnRect();
	Rect getCancelBtnRect();

	void initializeSprites();
	void createSprites();
	void positionSprites();

	void updateTextBoxSprite();

	void checkButtonsMouseHover();
	void checkButtonsMouseHover(int x, int y);
	void checkButtonsMouseHover(const Point& mouse);

	void checkButtonsClick();
	void checkButtonsClick(int x, int y);
	void checkButtonsClick(const Point& mouse);

	void okBtnClick();
	void cancelBtnClick();
protected:
	void onStarted() override;
	void onActivated() override;
	void onDeactivated() override;
	void onKeyDown(KeyCode key) override;
	void onTextInput(const char* text) override;
	void onMouseBtnDown(MouseBtn btn, int x, int y) override;
	void onMouseMove(int x, int y) override;
	void onPaint(std::shared_ptr<ICanvas> canvas, const Rect& invalidRect)
		override;
public:
	StagePropertiesController(std::shared_ptr<ISysProxy> sysProxy,
		std::shared_ptr<StageEditor> stageEditor);
};

#endif // STAGEPROPERTIESCONTROLLER_HPP
