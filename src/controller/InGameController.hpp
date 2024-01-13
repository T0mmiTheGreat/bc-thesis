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
#include <vector>

#include "types.hpp"
#include "controller/GeneralControllerBase.hpp"
#include "core/ICore.hpp"
#include "sprite/PlayerSprite.hpp"

class InGameController : public GeneralControllerBase {
private:
	static constexpr int PLAYER_COUNT = 2;

	std::unique_ptr<ICore> m_core;
	std::vector<std::unique_ptr<PlayerSprite>> m_playerSprites;
	Timer m_tickTimer;

	/**
	 * @brief Sets the player sprite properties based on the state of the core.
	 */
	void updatePlayerSprites();
public:
	InGameController(std::shared_ptr<ISysProxy> sysProxy);
	void startedEvent() override;
	void loopEvent() override;
	void paintEvent(std::shared_ptr<ICanvas> canvas, Rect& invalidRect) override;
};

#endif // INGAMECONTROLLER_HPP
