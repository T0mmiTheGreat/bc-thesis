/**
 * @file StagePropertiesController.cpp
 * @author Tomáš Ludrovan
 * @brief StagePropertiesController class
 * @version 0.1
 * @date 2024-02-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "controller/StagePropertiesController.hpp"

#include "functions.hpp"

Rect StagePropertiesController::getOkBtnRect()
{
	Rect res(
		(sysProxy->getPaintAreaSize().w - TextButtonSprite::BUTTON_WIDTH) / 2,
		BUTTONS_TOP,
		TextButtonSprite::BUTTON_WIDTH,
		TextButtonSprite::BUTTON_HEIGHT
	);
	return res;
}

Rect StagePropertiesController::getCancelBtnRect()
{
	Rect res(
		(sysProxy->getPaintAreaSize().w - TextButtonSprite::BUTTON_WIDTH) / 2,
		// Top      + height of OK btn                + the space between buttons
		BUTTONS_TOP + TextButtonSprite::BUTTON_HEIGHT + BUTTONS_SPACING,
		TextButtonSprite::BUTTON_WIDTH,
		TextButtonSprite::BUTTON_HEIGHT
	);
	return res;
}

void StagePropertiesController::initializeSprites()
{
	createSprites();
	positionSprites();
	updateTextBoxSprite();
}

void StagePropertiesController::createSprites()
{
	m_textBoxSprite = std::make_unique<TextBoxSprite>(sysProxy);
	m_filenameSprite = std::make_unique<TextSprite>(sysProxy);
	m_okBtnSprite = std::make_unique<TextButtonSprite>(sysProxy);
	m_cancelBtnSprite = std::make_unique<TextButtonSprite>(sysProxy);

	m_textBoxSprite->setFont(TEXTBOX_FONT);
	m_filenameSprite->setFont(FILENAME_FONT);
	m_textBoxSprite->setColor(TEXTBOX_TEXT_COLOR);
	m_filenameSprite->setColor(FILENAME_TEXT_COLOR);
	m_okBtnSprite->setText(OK_BTN_TEXT);
	m_cancelBtnSprite->setText(CANCEL_BTN_TEXT);
}

void StagePropertiesController::positionSprites()
{
	int textBoxHeight = m_textBoxSprite->getBounds().h;
	int textFilenameHeight = m_filenameSprite->getBounds().h;
	int paintAreaW = sysProxy->getPaintAreaSize().w;
	Rect textBoxRect(
		(paintAreaW - TEXTBOX_WIDTH) / 2,
		TEXTBOX_TOP,
		TEXTBOX_WIDTH,
		textBoxHeight
	);
	Rect textFilenameRect(
		textBoxRect.x,
		textBoxRect.getBottom(),
		TEXTBOX_WIDTH,
		textFilenameHeight
	);

	m_textBoxSprite->setTextRect(textBoxRect);
	m_filenameSprite->setTextRect(textFilenameRect);
	m_okBtnSprite->setPos(getOkBtnRect().getTopLeft());
	m_cancelBtnSprite->setPos(getCancelBtnRect().getTopLeft());
}

void StagePropertiesController::updateTextBoxSprite()
{
	m_textBoxSprite->setText(m_stageTitle);
	
	std::string filename;
	bool isAutoGenerated;
	m_stageEditor->getStageFileName(m_stageTitle, filename, isAutoGenerated);
	
	std::string filenameSpriteText = "File name";
	if (isAutoGenerated) {
		filenameSpriteText += " (auto)";
	}
	filenameSpriteText += ": " + filename;

	m_filenameSprite->setText(filenameSpriteText);
}

void StagePropertiesController::checkButtonsMouseHover()
{
	checkButtonsMouseHover(sysProxy->getMousePos());
}

void StagePropertiesController::checkButtonsMouseHover(int x, int y)
{
	checkButtonsMouseHover(Point(x, y));
}

void StagePropertiesController::checkButtonsMouseHover(const Point& mouse)
{
	if (getOkBtnRect().containsPoint(mouse)) {
		m_okBtnSprite->setCostume(TextButtonSprite::COSTUME_HOVER);
	} else {
		m_okBtnSprite->setCostume(TextButtonSprite::COSTUME_NORMAL);
	}

	if (getCancelBtnRect().containsPoint(mouse)) {
		m_cancelBtnSprite->setCostume(TextButtonSprite::COSTUME_HOVER);
	} else {
		m_cancelBtnSprite->setCostume(TextButtonSprite::COSTUME_NORMAL);
	}
}

void StagePropertiesController::checkButtonsClick()
{
	checkButtonsClick(sysProxy->getMousePos());
}

void StagePropertiesController::checkButtonsClick(int x, int y)
{
	checkButtonsClick(Point(x, y));
}

void StagePropertiesController::checkButtonsClick(const Point& mouse)
{
	if (getOkBtnRect().containsPoint(mouse)) {
		okBtnClick();
	} else if (getCancelBtnRect().containsPoint(mouse)) {
		cancelBtnClick();
	}
}

void StagePropertiesController::okBtnClick()
{
	m_stageEditor->setStageTitle(m_stageTitle);
	finish();
}

void StagePropertiesController::cancelBtnClick()
{
	finish();
}

StagePropertiesController::StagePropertiesController(
	std::shared_ptr<ISysProxy> sysProxy,
	std::shared_ptr<StageEditor> stageEditor)
	: GeneralControllerBase(sysProxy)
	, m_stageEditor{stageEditor}
{}

void StagePropertiesController::startedEvent()
{
	GeneralControllerBase::startedEvent();
	
	m_stageTitle = m_stageEditor->getState().title;

	initializeSprites();
}

void StagePropertiesController::activatedEvent()
{
	GeneralControllerBase::activatedEvent();
	sysProxy->setTextInputEnabled(true);
}

void StagePropertiesController::deactivatedEvent()
{
	sysProxy->setTextInputEnabled(false);
	GeneralControllerBase::deactivatedEvent();
}

void StagePropertiesController::keyDownEvent(KeyCode key)
{
	if (key == KEY_BACKSPACE) {
		// Delete character

		// Is there anything to delete?
		if (!m_stageTitle.empty()) {
			popBackUtf8(m_stageTitle);
			updateTextBoxSprite();
		}
	}
}

void StagePropertiesController::textInputEvent(const char* text)
{
	m_stageTitle += text;
	updateTextBoxSprite();
}

void StagePropertiesController::mouseBtnDownEvent(MouseBtn btn, int x, int y)
{
	if (btn == BTN_LEFT) {
		checkButtonsClick(x, y);
	}
}

void StagePropertiesController::mouseMoveEvent(int x, int y)
{
	checkButtonsMouseHover(x, y);
}

void StagePropertiesController::paintEvent(std::shared_ptr<ICanvas> canvas,
	const Rect& invalidRect)
{
	m_textBoxSprite->repaint(canvas, invalidRect);
	m_filenameSprite->repaint(canvas, invalidRect);
	m_okBtnSprite->repaint(canvas, invalidRect);
	m_cancelBtnSprite->repaint(canvas, invalidRect);
}