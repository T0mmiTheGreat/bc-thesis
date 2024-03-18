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
#include <unordered_set>
#include <vector>

#include "types.hpp"
#include "controller/GeneralControllerBase.hpp"
#include "core/Common.hpp"
#include "core/Core.hpp"
#include "core/CoreAction.hpp"
#include "gamesetupdata/GameSetupData.hpp"
#include "sprite/PlayerSprite.hpp"
#include "sprite/ObstacleSprite.hpp"
#include "sprite/BonusSprite.hpp"
#include "sprite/BonusHpRecoverySprite.hpp"
#include "sprite/HollowRectSprite.hpp"
#include "sprite/OptionBarSprite.hpp"
#include "sprite/OptionButtonSprite.hpp"
#include "sprite/TextSprite.hpp"
#include "stageviewport/StageViewport.hpp"

class InGameController : public GeneralControllerBase {
private:
	// Internally, full HP is equal to `1.0`. Externally (as shown to the user),
	// full HP is equal to `100.0`.
	static constexpr double PLAYER_HP_FACTOR = 100.0;

	static constexpr int STAGE_AREA_LEFT = 0;
	static constexpr int STAGE_AREA_TOP = 0;
	static constexpr int MIN_STAGE_LEFT_MARGIN = 16;
	static constexpr int MIN_STAGE_TOP_MARGIN = MIN_STAGE_LEFT_MARGIN;
	static constexpr int MIN_STAGE_RIGHT_MARGIN = MIN_STAGE_LEFT_MARGIN;
	static constexpr int MIN_STAGE_BOTTOM_MARGIN = MIN_STAGE_TOP_MARGIN;

	static constexpr int STATUS_BAR_WIDTH = 180;
	static constexpr int EXIT_BTN_BOTTOM_MARGIN = 30;
	static constexpr int PLAYER_HP_RIGHT_MARGIN = 14;
	static constexpr int PLAYER_HP_TOP_MARGIN = 8;
	static constexpr int PLAYER_HP_BOTTOM_MARGIN = PLAYER_HP_TOP_MARGIN;

	static constexpr FontId PLAYER_HP_FONT = FONT_MCLAREN_20;

	static constexpr const char* EXIT_BTN_TEXT = "Quit";

	static constexpr Color GAME_OVER_TEXT_COLOR = Color::white();
	static constexpr FontId GAME_OVER_TEXT_FONT = FONT_JULEE_66;
	static constexpr const char* GAME_OVER_TEXT_WINNER = "WINNER";
	static constexpr const char* GAME_OVER_TEXT_DRAW_GAME = "DRAW GAME";

	std::unordered_map<PlayerId, std::unique_ptr<PlayerSprite>> m_playerSprites;
	std::vector<std::unique_ptr<ObstacleSprite>> m_obstacleSprites;
	std::unordered_map<BonusId, std::unique_ptr<BonusSprite>> m_bonusSprites;
	std::unordered_set<std::unique_ptr<BonusHpRecoverySprite>>
	m_bonusHpRecoverySprites;
	std::unique_ptr<HollowRectSprite> m_stageBoundsSprite;

	std::unique_ptr<OptionBarSprite> m_statusBarSprite;
	std::unordered_map<PlayerId, std::unique_ptr<OptionBarSprite>>
	m_playerHpBgSprites;
	std::unordered_map<PlayerId, std::unique_ptr<TextSprite>>
	m_playerHpTextSprites;
	std::unique_ptr<OptionButtonSprite> m_exitBtnSprite;
	std::unique_ptr<TextSprite> m_gameOverTextSprite;

	std::unique_ptr<Core> m_core;
	std::unique_ptr<StageViewport> m_viewport;

	void createPlayerSprite(PlayerId id);
	void createBonusSprite(BonusId id, const PointF& pos);
	void createPlayerHpBgSprite(PlayerId id);
	void createPlayerHpTextSprite(PlayerId id);
	void createObstacleSprite(const PolygonF& shape);

	void createSprites();
	void createStageBoundsSprite();
	void createStatusBarSprite();
	void createExitBtnSprite();
	void createGameOverTextSprite();

	void checkExitBtnMouseHover(const Point& mouse);
	void checkExitBtnClick(const Point& mouse);
	void exitBtnClick();

	void updatePlayerPos(PlayerId id, const PointF& pos);
	void updatePlayerHp(PlayerId id, double hp);
	void updatePlayerSize(PlayerId id, double size);

	void updateHpRecoverySprites();

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
	void updateSpritesByActionRemovePlayer(
		const std::shared_ptr<CoreAction> action);
	void updateSpritesByActionSetPlayerPos(
		const std::shared_ptr<CoreAction> action);
	void updateSpritesByActionSetPlayerHp(
		const std::shared_ptr<CoreAction> action);
	void updateSpritesByActionSetPlayerSize(
		const std::shared_ptr<CoreAction> action);
	void updateSpritesByActionAddBonus(
		const std::shared_ptr<CoreAction> action);
	void updateSpritesByActionRemoveBonus(
		const std::shared_ptr<CoreAction> action);
	void updateSpritesByActionAnnounceWinner(
		const std::shared_ptr<CoreAction> action);
	void updateSpritesByActionAnnounceDrawGame(
		const std::shared_ptr<CoreAction> action);

	void initializeViewport();

	Rect getStageAreaRect();
	Rect getPlayerHpBarRect(PlayerId playerId);
	int getPlayerHpTextHeight();
protected:
	std::shared_ptr<IControllerChild> createReplacement() override;

	void onStarted() override;
	void onLoop() override;
	void onMouseMove(int x, int y) override;
	void onMouseBtnDown(MouseBtn btn, int x, int y) override;
	void onPaint(std::shared_ptr<ICanvas> canvas,
		const Rect& invalidRect) override;
public:
	InGameController(std::shared_ptr<ISysProxy> sysProxy,
		const GameSetupData& gsdata);
};

#endif // INGAMECONTROLLER_HPP
