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
#include "playerinput/IPlayerInput.hpp"
#include "sprite/PlayerSprite.hpp"
#include "sprite/ObstacleSprite.hpp"
#include "sprite/HollowRectSprite.hpp"
#include "stageserializer/IStageSerializer.hpp"

class InGameController : public GeneralControllerBase {
private:
	std::unique_ptr<Core> m_core;
	std::unordered_map<PlayerId, std::unique_ptr<PlayerSprite>> m_playerSprites;
	std::vector<std::unique_ptr<ObstacleSprite>> m_obstacleSprites;
	std::unique_ptr<HollowRectSprite> m_stageBoundsSprite;

	void createPlayerSprites();
	void createObstacleSprites();
	void createStageBoundsSprite();
	/**
	 * @brief Sets the player sprite properties based on the state of the core.
	 */
	void updatePlayerSprites();
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
