/**
 * @file Common.hpp
 * @author Tomáš Ludrovan
 * @brief Common definitions
 * @version 0.1
 * @date 2024-02-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef CORE_COMMON_HPP
#define CORE_COMMON_HPP

#include <ctime>

#include "types.hpp"

typedef TriangleF StageObstacle;

/**
 * @brief The number of milliseconds between ticks.
 * 
 * @details I.e., how often the tick() events should be sent.
 */
constexpr std::clock_t TICK_INTERVAL = 17;

#endif // CORE_COMMON_HPP
