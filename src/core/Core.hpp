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

	PlayerList m_players;

	void playerTick(std::shared_ptr<IPlayerState> player);
	static void inputToVector(const PlayerInputState& input, double& x, double& y);
public:
	void tick() override;
	void addPlayer(std::shared_ptr<IPlayerState> player) override;
	const PlayerList& getPlayerList() override;
};

#endif // CORE_HPP
