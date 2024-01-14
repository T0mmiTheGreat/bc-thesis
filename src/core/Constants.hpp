/**
 * @file Constants.hpp
 * @author Tomáš Ludrovan
 * @brief Core-related constants
 * @version 0.1
 * @date 2024-01-14
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <ctime>

/**
 * @brief The number of milliseconds between ticks.
 * 
 * @details I.e., how often the tick() events should be sent.
 */
constexpr std::clock_t TICK_INTERVAL = 17; 

#endif // CONSTANTS_HPP
