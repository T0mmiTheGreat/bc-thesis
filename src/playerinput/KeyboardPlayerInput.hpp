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
	/**
	 * @brief Constructs a new PlayerInputFactory object.
	 * 
	 * @param keyLeft Key code for the "move left" action.
	 * @param keyUp Key code for the "move up" action.
	 * @param keyRight Key code for the "move right" action.
	 * @param keyDown Key code for the "move down" action.
	 */
	KeyboardPlayerInput(KeyCode keyLeft, KeyCode keyUp, KeyCode keyRight,
		KeyCode keyDown);
	static KeyboardPlayerInput constructWSAD();
	static KeyboardPlayerInput constructArrows();
	PlayerInputState readInput() override;
};

#endif // KEYBOARDPLAYERINPUT_HPP
