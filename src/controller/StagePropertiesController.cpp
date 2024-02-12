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
	m_okBtnSprite = std::make_unique<TextButtonSprite>(sysProxy);
	m_cancelBtnSprite = std::make_unique<TextButtonSprite>(sysProxy);

	m_okBtnSprite->setText(OK_BTN_TEXT);
	m_cancelBtnSprite->setText(CANCEL_BTN_TEXT);
}

void StagePropertiesController::positionSprites()
{
	int textBoxHeight = m_textBoxSprite->getBounds().h;
	int paintAreaW = sysProxy->getPaintAreaSize().w;
	Rect textBoxRect(
		(paintAreaW - TEXTBOX_WIDTH) / 2,
		TEXTBOX_TOP,
		TEXTBOX_WIDTH,
		textBoxHeight
	);

	m_textBoxSprite->setTextRect(textBoxRect);
	m_okBtnSprite->setPos(getOkBtnRect().getTopLeft());
	m_cancelBtnSprite->setPos(getCancelBtnRect().getTopLeft());
}

void StagePropertiesController::updateTextBoxSprite()
{
	m_textBoxSprite->setText(m_stageTitle);
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
		if (!m_stageTitle.empty()) {
			m_stageTitle.pop_back();
			while (m_stageTitle.back() & 0x80) {
				m_stageTitle.pop_back();
			}
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
	(void)btn;
	(void)x;
	(void)y;
	// TODO
}

void StagePropertiesController::mouseMoveEvent(int x, int y)
{
	(void)x;
	(void)y;
	// TODO
}

void StagePropertiesController::paintEvent(std::shared_ptr<ICanvas> canvas,
	const Rect& invalidRect)
{
	m_textBoxSprite->repaint(canvas, invalidRect);
	m_okBtnSprite->repaint(canvas, invalidRect);
	m_cancelBtnSprite->repaint(canvas, invalidRect);
}
