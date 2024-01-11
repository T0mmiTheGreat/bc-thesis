/**
 * @file ICore.hpp
 * @author Tomáš Ludrovan
 * @brief ICore interface
 * @version 0.1
 * @date 2023-12-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef ICORE_HPP
#define ICORE_HPP

#include <ctime>
#include <memory>
#include <vector>

#include "playerstate/IPlayerState.hpp"

// The type might change, but it will always meet the named requirement
// "Container" and the element type will be `*IPlayerState` (may change to
// a different pointer though; e.g., weak, unique, ...)
typedef std::vector<std::shared_ptr<IPlayerState>> PlayerList;

/**
 * @brief The game's core.
 * 
 * @details Create, add players, and then send tick events to progress the game.
 */
class ICore {
public:
	/**
	 * @brief The number of milliseconds between ticks.
	 * 
	 * @details I.e., how often the tick() events should be sent.
	 */
	static constexpr std::clock_t TICK_INTERVAL = 17;

	virtual ~ICore() {}
	/**
	 * @brief Game tick event.
	 * 
	 * @details Game tick is the moment when the game state progresses further.
	 *          Players move, bonuses appear, effects are applied, etc.
	 */
	virtual void tick() = 0;
	/**
	 * @brief Add a player to the player list.
	 */
	virtual void addPlayer(std::shared_ptr<IPlayerState> player) = 0;
	/**
	 * @brief Access the player list.
	 */
	virtual const PlayerList& getPlayerList() = 0;
};

#endif // ICORE_HPP
