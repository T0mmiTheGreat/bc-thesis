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
#include "playerinput/IPlayerInput.hpp"
#include "sprite/PlayerSprite.hpp"

class InGameController : public GeneralControllerBase {
private:
	static constexpr int PLAYER_COUNT = 3;

	std::unique_ptr<ICore> m_core;
	std::vector<std::unique_ptr<PlayerSprite>> m_playerSprites;

	void createPlayers();
	void newPlayer(std::shared_ptr<IPlayerInput> playerInput, double startX,
		double startY, const Color& color);
	/**
	 * @brief Sets the player sprite properties based on the state of the core.
	 */
	void updatePlayerSprites();
public:
	InGameController(std::shared_ptr<ISysProxy> sysProxy);
	void startedEvent() override;
	void loopEvent() override;
	void paintEvent(std::shared_ptr<ICanvas> canvas, const Rect& invalidRect) override;
};

#endif // INGAMECONTROLLER_HPP
