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

typedef size_t PlayerId;
typedef size_t BonusId;

typedef TriangleF StageObstacle;

constexpr BonusId BONUS_ID_NULL = 0;

/**
 * @brief The number of milliseconds between ticks.
 * 
 * @details I.e., how often the tick() events should be sent.
 */
constexpr std::clock_t TICK_INTERVAL = 17;

constexpr double BONUS_RADIUS = 25.0;

#endif // CORE_COMMON_HPP
