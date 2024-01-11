/**
 * @file Core.hpp
 * @author Tomáš Ludrovan
 * @brief Core class
 * @version 0.1
 * @date 2023-12-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef CORE_HPP
#define CORE_HPP

#include "core/ICore.hpp"

class Core : public ICore {
private:
	// sqrt(1/2)
	// sin(pi/4)
	static constexpr double DIAG_MOVEMENT_PERAXIS_LENGTH = 0.70710678118654752440084436210485;
	// Value to multiply with the player speed
	static constexpr double SPEED_FACTOR = TICK_INTERVAL / 6.0;
	// HP depleted per frame
	static constexpr double HP_DRAIN = TICK_INTERVAL / 3400.0;

	/**
	 * @brief Player list.
	 */
	PlayerList m_players;

	/**
	 * @brief "Tick" a player.
	 * 
	 * @param player The player to "tick".
	 */
	void playerTick(std::shared_ptr<IPlayerState> player);
	/**
	 * @brief Convert player input value to a vector.
	 * 
	 * @details Based on the player input creates a unit vector in the direction
	 *          the player wants to move.
	 * 
	 * @param input The player input.
	 * @param x Vector X coordinate.
	 * @param y Vector Y coordinate.
	 */
	static void inputToVector(const PlayerInputState& input, double& x, double& y);
public:
	void tick() override;
	void addPlayer(std::shared_ptr<IPlayerState> player) override;
	const PlayerList& getPlayerList() override;
};

#endif // CORE_HPP
