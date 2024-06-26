/**
 * @file AIPlayerAgentBase.hpp
 * @author Tomáš Ludrovan
 * @brief AIPlayerAgentBase class
 * @version 0.1
 * @date 2024-03-25
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef AIPLAYERAGENTBASE_HPP
#define AIPLAYERAGENTBASE_HPP

//#define USE_THREADS_FOR_AGENTS

#ifdef USE_THREADS_FOR_AGENTS
#include <condition_variable>
#include <mutex>
#include <thread>
#endif // USE_THREADS_FOR_AGENTS

#include "aiplayeragent/IAIPlayerAgent.hpp"
#include "core/Common.hpp"

class AIPlayerAgentBase : public IAIPlayerAgent {
private:
#ifdef USE_THREADS_FOR_AGENTS
	// The thread in which the agent runs
	std::thread m_thr;
#endif // USE_THREADS_FOR_AGENTS
	// Whether the `kill()` method has been called
	bool m_isThreadFinished;

#ifdef USE_THREADS_FOR_AGENTS
	// True if the thread is currently inside the `doPlan()` method
	bool m_isPlanning;
	std::condition_variable m_cvIsPlanning;
	std::mutex m_mutexIsPlanning;

	// If false, the thread will be blocked at the beggining of its loop
	bool m_isPlanNotify;
	std::condition_variable m_cvIsPlanNotify;
	std::mutex m_mutexIsPlanNotify;

	/**
	 * @brief The main function of the `m_t` thread.
	 */
	void threadMain();

	/**
	 * @brief (Passively) waits until a condition becomes true.
	 * 
	 * @tparam Predicate Must meet the requirements of `FunctionObject`
	 *                   whose return value is convertible to `bool`.
	 * @param m The mutex to lock.
	 * @param cv The condition variable to wait at.
	 * @param pred The condition to wait for it becoming true.
	 */
	template <typename Predicate>
	void waitForCondition(std::mutex& m, std::condition_variable& cv,
		Predicate pred);
	/**
	 * @brief Updates a condition value.
	 * 
	 * @tparam Predicate Must be callable.
	 * @param m The mutex to lock.
	 * @param cv The condition variable to notify.
	 * @param pred Callable which modifies the condition state.
	 */
	template <typename Predicate>
	void updateCondition(std::mutex& m, std::condition_variable& cv,
		Predicate pred);
#endif // USE_THREADS_FOR_AGENTS
protected:
	GameStateAgentProxyP gsProxy;
	const PlayerId myId;

	/**
	 * @brief Returns player state which belongs to this agent.
	 */
	const GameStateAgentProxy::PlayerState& getMyState() const;
	
	static std::vector<PlayerInputFlags> generateInputs();

	/**
	 * @brief Performs planning.
	 * 
	 * @details In the worst case, the execution time should be `TICK_INTERVAL
	 *          / MAX_PLAYERS` (17 / 8 ~ 2 ms).
	 */
	virtual void doPlan() = 0;
	/**
	 * @brief Returns the desired agent input based on previous planning.
	 */
	virtual PlayerInputFlags doGetPlayerInput() = 0;
public:
	AIPlayerAgentBase(PlayerId myId);
	~AIPlayerAgentBase();
	PlayerInputFlags getPlayerInput() override;
	void plan() override;
	void assignProxy(GameStateAgentProxyP value) override;
	void kill() override;
};

#ifdef USE_THREADS_FOR_AGENTS

template<typename Predicate>
inline void AIPlayerAgentBase::waitForCondition(std::mutex& m,
	std::condition_variable& cv, Predicate pred)
{
	std::unique_lock<std::mutex> lk(m);
	cv.wait(lk, pred);
}

template<typename Predicate>
inline void AIPlayerAgentBase::updateCondition(std::mutex& m,
	std::condition_variable& cv, Predicate pred)
{
	{
		std::lock_guard<std::mutex> lk(m);
		pred();
	}
	cv.notify_all();
}

#endif // USE_THREADS_FOR_AGENTS

#endif // AIPLAYERAGENTBASE_HPP
