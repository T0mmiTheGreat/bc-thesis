/**
 * @file ISysProxyPlayerInput.hpp
 * @author Tomáš Ludrovan
 * @brief ISysProxyPlayerInput interface
 * @version 0.1
 * @date 2024-01-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef ISYSPROXYPLAYERINPUT_HPP
#define ISYSPROXYPLAYERINPUT_HPP

#include "types.hpp"

/**
 * @brief Subset of sys proxy used by PlayerInput classes.
 */
class ISysProxyPlayerInput {
public:
	virtual ~ISysProxyPlayerInput() {}
	/**
	 * @brief Returns true if the key represented by `keyCode` is pressed.
	 */
	virtual bool isKeyPressed(KeyCode keyCode) = 0;
};

#endif // ISYSPROXYPLAYERINPUT_HPP
