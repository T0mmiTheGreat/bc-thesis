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
#include "sprite/TextBoxSprite.hpp"
#include "sprite/TextButtonSprite.hpp"
#include "stageeditor/StageEditor.hpp"

class StagePropertiesController : public GeneralControllerBase {
private:
	static constexpr int TEXTBOX_TOP = 220;
	static constexpr int TEXTBOX_WIDTH = 220;
	static constexpr int BUTTONS_TOP = 272;
	static constexpr int BUTTONS_SPACING = 8;

	static constexpr std::string OK_BTN_TEXT = "OK";
	static constexpr std::string CANCEL_BTN_TEXT = "Cancel";

	std::shared_ptr<StageEditor> m_stageEditor;
	std::string m_stageTitle;
	std::unique_ptr<TextBoxSprite> m_textBoxSprite;
	std::unique_ptr<TextButtonSprite> m_okBtnSprite;
	std::unique_ptr<TextButtonSprite> m_cancelBtnSprite;

	Rect getOkBtnRect();
	Rect getCancelBtnRect();

	void initializeSprites();
	void createSprites();
	void positionSprites();

	void updateTextBoxSprite();
public:
	StagePropertiesController(std::shared_ptr<ISysProxy> sysProxy,
		std::shared_ptr<StageEditor> stageEditor);
	void startedEvent() override;
	void activatedEvent() override;
	void deactivatedEvent() override;
	void keyDownEvent(KeyCode key) override;
	void textInputEvent(const char* text) override;
	void mouseBtnDownEvent(MouseBtn btn, int x, int y) override;
	void mouseMoveEvent(int x, int y) override;
	void paintEvent(std::shared_ptr<ICanvas> canvas, const Rect& invalidRect)
		override;
};

#endif // STAGEPROPERTIESCONTROLLER_HPP
