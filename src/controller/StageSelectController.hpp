/**
 * @file StageSelectController.hpp
 * @author Tomáš Ludrovan
 * @brief StageSelectController class
 * @version 0.1
 * @date 2024-02-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef STAGESELECTCONTROLLER_HPP
#define STAGESELECTCONTROLLER_HPP

#include <memory>
#include <string>
#include <vector>

#include "controller/GeneralControllerBase.hpp"
#include "sprite/StageItemSprite.hpp"
#include "sprite/OptionButtonSprite.hpp"
#include "stageserializer/IStageSerializer.hpp"
#include "sysproxy/ISysProxy.hpp"

class StageSelectController : public GeneralControllerBase {
private:
	// Relative to screen width
	static constexpr double STAGE_ITEM_WIDTH_REL = 0.6;
	static constexpr int STAGE_ITEM_TOP_MARGIN = 50;
	static constexpr int STAGE_ITEM_DISTANCES = 90;
	static constexpr int CANCEL_BUTTON_BOTTOM_MARGIN = 110;
	static constexpr std::string CANCEL_BUTTON_TEXT = "Cancel";

	// Return values

	IStageSerializer::IdType& m_selectedStage;
	bool& m_isSelectedStageValid;

	std::vector<IStageSerializer::IdType> m_stageIds;

	std::unique_ptr<OptionButtonSprite> m_cancelButtonSprite;
	std::vector<std::unique_ptr<StageItemSprite>> m_stageItemSprites;

	void initializeSprites();
	void createSprites();
	void positionSprites();

	void cancelButtonClick();
	void stageItemClick(int itemIdx);

	void updateAllSprites();
	void updateStagesList();
	void addStageItemSprite(const std::string& title, const std::string& path);

	Rect getStageItemRect(int itemIdx);
	Rect getCancelButtonRect();

	void checkCancelButtonMouseHover(const Point& mouse);
	void checkStageItemMouseHover(const Point& mouse);

	void checkCancelButtonClick(const Point& mouse);
	void checkStageItemClick(const Point& mouse);

	int getStageItemWidth();
protected:
	void onStarted() override;
	void onMouseMove(int x, int y) override;
	void onMouseBtnDown(MouseBtn btn, int x, int y) override;
	void onPaint(std::shared_ptr<ICanvas> canvas, const Rect& invalidRect) override;
public:
	StageSelectController(std::shared_ptr<ISysProxy> sysProxy,
		IStageSerializer::IdType& selectedStage, bool& isSelectedStageValid);
};

#endif // STAGESELECTCONTROLLER_HPP
