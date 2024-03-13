/**
 * @file InGameController.hpp
 * @author Tomáš Ludrovan
 * @brief InGameController class
 * @version 0.1
 * @date 2023-12-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef INGAMECONTROLLER_HPP
#define INGAMECONTROLLER_HPP

#include <ctime>
#include <memory>
#include <unordered_map>
#include <vector>

#include "types.hpp"
#include "controller/GeneralControllerBase.hpp"
#include "core/Common.hpp"
#include "core/Core.hpp"
#include "core/CoreAction.hpp"
#include "playerinput/IPlayerInput.hpp"
#include "sprite/PlayerSprite.hpp"
#include "sprite/ObstacleSprite.hpp"
#include "sprite/BonusSprite.hpp"
#include "sprite/HollowRectSprite.hpp"
#include "sprite/OptionBarSprite.hpp"
#include "sprite/TextSprite.hpp"
#include "stageserializer/IStageSerializer.hpp"
#include "stageviewport/StageViewport.hpp"

class InGameController : public GeneralControllerBase {
private:
	static constexpr int STAGE_AREA_LEFT = 0;
	static constexpr int STAGE_AREA_TOP = 0;
	static constexpr int MIN_STAGE_LEFT_MARGIN = 16;
	static constexpr int MIN_STAGE_TOP_MARGIN = MIN_STAGE_LEFT_MARGIN;
	static constexpr int MIN_STAGE_RIGHT_MARGIN = MIN_STAGE_LEFT_MARGIN;
	static constexpr int MIN_STAGE_BOTTOM_MARGIN = MIN_STAGE_TOP_MARGIN;

	static constexpr int STATUS_BAR_WIDTH = 180;
	static constexpr int PLAYER_HP_RIGHT_MARGIN = 14;
	static constexpr int PLAYER_HP_TOP_MARGIN = 8;
	static constexpr int PLAYER_HP_BOTTOM_MARGIN = PLAYER_HP_TOP_MARGIN;

	static constexpr FontId PLAYER_HP_FONT = FONT_BRLNSTR_20;

	std::unordered_map<PlayerId, std::unique_ptr<PlayerSprite>> m_playerSprites;
	std::vector<std::unique_ptr<ObstacleSprite>> m_obstacleSprites;
	std::unordered_map<BonusId, std::unique_ptr<BonusSprite>> m_bonusSprites;
	std::unique_ptr<HollowRectSprite> m_stageBoundsSprite;

	std::unique_ptr<OptionBarSprite> m_statusBarSprite;
	std::unordered_map<PlayerId, std::unique_ptr<OptionBarSprite>>
	m_playerHpBgSprites;
	std::unordered_map<PlayerId, std::unique_ptr<TextSprite>>
	m_playerHpTextSprites;

	std::unique_ptr<Core> m_core;
	std::unique_ptr<StageViewport> m_viewport;

	void createPlayerSprite(PlayerId id);
	void createBonusSprite(BonusId id, const PointF& pos);
	void createPlayerHpBgSprite(PlayerId id);
	void createPlayerHpTextSprite(PlayerId id);
	void createObstacleSprite(const PolygonF& shape);
	void createStageBoundsSprite();
	void createStatusBarSprite();

	void updatePlayerPos(PlayerId id, const PointF& pos);
	void updatePlayerHp(PlayerId id, double hp);
	void updatePlayerSize(PlayerId id, double size);

	void updateSpritesByAction(
		const std::shared_ptr<CoreAction> action);
	void updateSpritesByActionNone(
		const std::shared_ptr<CoreAction> action);
	void updateSpritesByActionMultiple(
		const std::shared_ptr<CoreAction> action);
	void updateSpritesByActionAddObstacle(
		const std::shared_ptr<CoreAction> action);
	void updateSpritesByActionSetStageSize(
		const std::shared_ptr<CoreAction> action);
	void updateSpritesByActionAddPlayer(
		const std::shared_ptr<CoreAction> action);
	void updateSpritesByActionSetPlayerPos(
		const std::shared_ptr<CoreAction> action);
	void updateSpritesByActionSetPlayerHp(
		const std::shared_ptr<CoreAction> action);
	void updateSpritesByActionSetPlayerSize(
		const std::shared_ptr<CoreAction> action);
	void updateSpritesByActionAddBonus(
		const std::shared_ptr<CoreAction> action);

	void initializeViewport();

	Rect getStageAreaRect();
	Rect getPlayerHpBarRect(PlayerId playerId);
	int getPlayerHpTextHeight();
protected:
	void onStarted() override;
	void onLoop() override;
	void onPaint(std::shared_ptr<ICanvas> canvas,
		const Rect& invalidRect) override;
public:
	InGameController(std::shared_ptr<ISysProxy> sysProxy,
		const std::shared_ptr<IStageSerializer> stage,
		const std::vector<std::shared_ptr<IPlayerInput>>& players);
};

#endif // INGAMECONTROLLER_HPP
