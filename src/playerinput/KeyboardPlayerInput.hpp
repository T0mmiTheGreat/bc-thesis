/**
 * @file KeyboardPlayerInput.hpp
 * @author Tomáš Ludrovan
 * @brief KeyboardPlayerInput class
 * @version 0.1
 * @date 2023-12-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef KEYBOARDPLAYERINPUT_HPP
#define KEYBOARDPLAYERINPUT_HPP

#include <memory>

#include "types.hpp"
#include "playerinput/PlayerInputBase.hpp"
#include "playerinput/ISysProxyPlayerInput.hpp"

class KeyboardPlayerInput : public PlayerInputBase {
private:
	std::shared_ptr<ISysProxyPlayerInput> m_sysProxy;
	KeyCode m_keyLeft;
	KeyCode m_keyUp;
	KeyCode m_keyRight;
	KeyCode m_keyDown;
	KeyCode m_keyDeflate;
public:
	/**
	 * @brief Constructs a new PlayerInputFactory object.
	 * 
	 * @param sysProxy 
	 * @param keyLeft Key code for the "move left" action.
	 * @param keyUp Key code for the "move up" action.
	 * @param keyRight Key code for the "move right" action.
	 * @param keyDown Key code for the "move down" action.
	 * @param keyDeflate Key code for the "deflate" action.
	 */
	KeyboardPlayerInput(std::shared_ptr<ISysProxyPlayerInput> sysProxy,
		KeyCode keyLeft, KeyCode keyUp, KeyCode keyRight, KeyCode keyDown,
		KeyCode keyDeflate);
	static KeyboardPlayerInput constructWSADQ(
		std::shared_ptr<ISysProxyPlayerInput> sysProxy);
	static KeyboardPlayerInput constructIKJLU(
		std::shared_ptr<ISysProxyPlayerInput> sysProxy);
	static KeyboardPlayerInput constructArrowsShift(
		std::shared_ptr<ISysProxyPlayerInput> sysProxy);
	PlayerInputFlags readInput() override;
};

#endif // KEYBOARDPLAYERINPUT_HPP
