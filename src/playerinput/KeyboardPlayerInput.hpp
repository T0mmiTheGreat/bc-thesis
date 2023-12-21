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

#include "types.hpp"
#include "playerinput/PlayerInputBase.hpp"

class KeyboardPlayerInput : public PlayerInputBase {
private:
	KeyCode m_keyLeft;
	KeyCode m_keyUp;
	KeyCode m_keyRight;
	KeyCode m_keyDown;
public:
	KeyboardPlayerInput(KeyCode keyLeft, KeyCode keyUp, KeyCode keyRight,
		KeyCode keyDown);
	static KeyboardPlayerInput constructWSAD();
	static KeyboardPlayerInput constructArrows();
	PlayerInputState readInput() override;
};

#endif // KEYBOARDPLAYERINPUT_HPP
