/**
 * @file AIPlayerAgentBase.cpp
 * @author Tomáš Ludrovan
 * @brief AIPlayerAgentBase class
 * @version 0.1
 * @date 2024-03-25
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "aiplayeragent/AIPlayerAgentBase.hpp"

#include <cassert>

#ifdef INCLUDE_BENCHMARK
#include <sstream>
#include <string>

#include "utilities/benchmark/Benchmark.hpp"

static constexpr const char* BENCH_ID_PLAN = "agent-plan-";
#endif // INCLUDE_BENCHMARK

#ifdef USE_THREADS_FOR_AGENTS

void AIPlayerAgentBase::threadMain()
{
	while (true) {
		// Wait until you can start planning (or until you're killed)
		waitForCondition(m_mutexIsPlanNotify, m_cvIsPlanNotify,
			[this](){ return this->m_isPlanNotify; });
		// Immediately prevent yourself from planning again
		updateCondition(m_mutexIsPlanNotify, m_cvIsPlanNotify,
			[this](){ this->m_isPlanNotify = false; });

		// Check if you're dead
		if (m_isThreadFinished) break;

		// Start planning
		updateCondition(m_mutexIsPlanning, m_cvIsPlanning,
			[this](){ this->m_isPlanning = true; });
		// Do the planning
		doPlan();
		// Stop planning
		updateCondition(m_mutexIsPlanning, m_cvIsPlanning,
			[this](){ this->m_isPlanning = false; });
	}
}

#endif // USE_THREADS_FOR_AGENTS

AIPlayerAgentBase::AIPlayerAgentBase(PlayerId myId_)
#ifdef USE_THREADS_FOR_AGENTS
	: m_thr(&threadMain, this)
	, m_isThreadFinished{false}

	, m_isPlanning{false}
	, m_cvIsPlanning()
	, m_mutexIsPlanning()

	, m_isPlanNotify{false}
	, m_cvIsPlanNotify()
	, m_mutexIsPlanNotify()

	, gsProxy{nullptr}
	, myId{myId_}
#else // !USE_THREADS_FOR_AGENTS
	: m_isThreadFinished{false}
	, gsProxy{nullptr}
	, myId{myId_}
#endif // !USE_THREADS_FOR_AGENTS
{}

const GameStateAgentProxy::PlayerState& AIPlayerAgentBase::getMyState() const
{
	return gsProxy->getPlayers().at(myId);
}

std::vector<PlayerInputFlags> AIPlayerAgentBase::generateInputs()
{
	static constexpr size_t NUM_INPUTS = 9;

	std::vector<PlayerInputFlags> res;
	res.reserve(NUM_INPUTS);

	res.push_back(PlayerInputFlags());
	res.push_back(PlayerInputFlags::createW());
	res.push_back(PlayerInputFlags::createNW());
	res.push_back(PlayerInputFlags::createN());
	res.push_back(PlayerInputFlags::createNE());
	res.push_back(PlayerInputFlags::createE());
	res.push_back(PlayerInputFlags::createSE());
	res.push_back(PlayerInputFlags::createS());
	res.push_back(PlayerInputFlags::createSW());

	return res;
}

AIPlayerAgentBase::~AIPlayerAgentBase()
{
	assert(m_isThreadFinished);
}

PlayerInputFlags AIPlayerAgentBase::getPlayerInput()
{
#ifdef USE_THREADS_FOR_AGENTS
	// Wait until the input is ready.
	// Ideally, there should be no waiting; the planning should be finished by
	// the time this method is called, but there might be a case that the thread
	// didn't get enough execution time from the OS, so we let it finish the
	// planning now.
	waitForCondition(m_mutexIsPlanning, m_cvIsPlanning,
		[this](){ return !this->m_isPlanning; });
#endif // USE_THREADS_FOR_AGENTS

	return doGetPlayerInput();
}

void AIPlayerAgentBase::plan()
{
#ifdef INCLUDE_BENCHMARK
	std::stringstream ss;
	ss << BENCH_ID_PLAN << static_cast<const void*>(this);
	Benchmark::get().beginMeasure(ss.str());
#endif // INCLUDE_BENCHMARK

#ifdef USE_THREADS_FOR_AGENTS
	// Wait until the previous planning is finished
	waitForCondition(m_mutexIsPlanning, m_cvIsPlanning,
		[this](){ return !this->m_isPlanning; });
	// Allow the agent thread to plan
	updateCondition(m_mutexIsPlanNotify, m_cvIsPlanNotify,
		[this](){ this->m_isPlanNotify = true; });
	// Wait until it actually starts planning
	waitForCondition(m_mutexIsPlanning, m_cvIsPlanning,
		[this](){ return this->m_isPlanning; });
#else // !USE_THREADS_FOR_AGENTS
	assert(gsProxy != nullptr);
	// If I am still alive
	if (gsProxy->getPlayers().find(myId) != gsProxy->getPlayers().end()) {
		doPlan();
	}
#endif // !USE_THREADS_FOR_AGENTS

#ifdef INCLUDE_BENCHMARK
	Benchmark::get().endMeasure(ss.str());
#endif // INCLUDE_BENCHMARK
}

void AIPlayerAgentBase::assignProxy(GameStateAgentProxyP value)
{
#ifndef USE_THREADS_FOR_AGENTS
	gsProxy = value;
#else // USE_THREADS_FOR_AGENTS
#	error Implement the game state proxy usage.
#endif
}

void AIPlayerAgentBase::kill()
{
	m_isThreadFinished = true;

#ifdef USE_THREADS_FOR_AGENTS
	// Allow the agent thread to check if it is alive
	updateCondition(m_mutexIsPlanNotify, m_cvIsPlanNotify,
		[this](){ this->m_isPlanNotify = true; });

	// Stop the thread
	m_thr.join();
#endif // USE_THREADS_FOR_AGENTS
}
