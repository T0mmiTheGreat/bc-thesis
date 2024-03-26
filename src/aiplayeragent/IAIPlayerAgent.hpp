/**
 * @file IAIPlayerAgent.hpp
 * @author Tomáš Ludrovan
 * @brief IAIPlayerAgent class
 * @version 0.1
 * @date 2024-03-25
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef IAIPLAYERAGENT_HPP
#define IAIPLAYERAGENT_HPP

#include "playerinput/IPlayerInput.hpp"

class IAIPlayerAgent {
public:
	/**
	 * @brief Destroys the IAIPlayerAgent object.
	 * 
	 * @details Before destroying, the `kill()` method must be called.
	 * 
	 * @note Not thread safe; a call to 'kill()` must be finished before
	 *       calling the destructor.
	 */
	virtual ~IAIPlayerAgent() {}
	/**
	 * @brief Returns the desired agent actions.
	 * 
	 * @details If the planning is not finished, waits until it is.
	 */
	virtual PlayerInputFlags getPlayerInput() = 0;
	/**
	 * @brief Tells the agent it may start planning.
	 * 
	 * @details The planning will be run in a separate thread. If the planning
	 *          is already running, waits until it's finished.
	 * 
	 * @note Not thread safe; a call to this method must be finished before
	 *       calling it again.
	 */
	virtual void plan() = 0;
	/**
	 * @brief Kill the agent.
	 * 
	 * @details This method must be called before calling the object destructor,
	 *          exactly once.
	 * 
	 * @note Not thread safe; a call to this method must be finished before
	 *       calling the destructor.
	 */
	virtual void kill() = 0;
};

#endif // IAIPLAYERAGENT_HPP
