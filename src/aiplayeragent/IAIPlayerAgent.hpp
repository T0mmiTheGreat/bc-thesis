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

#include "aiplayeragent/GameStateAgentProxy.hpp"
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
	 *          The game state proxy must have been assigned using the 
	 *          `assignProxy()` method before calling this method.
	 * 
	 * @note Not thread safe; a call to this method must be finished before
	 *       calling it again.
	 */
	virtual void plan() = 0;
	/**
	 * @brief Provides a game state proxy objects to the agent.
	 * 
	 * @details The agent has no means of planning if they don't have access to
	 *          the game state. The reason why it is not passed as a constructor
	 *          parameter is because generally the agents are created before the
	 *          proxy may exist (proxies are created in `Core` which gets the
	 *          agents as a constructor parameter).
	 */
	virtual void assignProxy(GameStateAgentProxyP value) = 0;
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
