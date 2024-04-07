/**
 * @file PlayerInputFlags.hpp
 * @author Tomáš Ludrovan
 * @brief PlayerInputFlags structure
 * @version 0.1
 * @date 2024-03-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef PLAYERINPUTFLAGS_HPP
#define PLAYERINPUTFLAGS_HPP

#include "math/Math.hpp"

/**
 * @brief Structure of flags representing the action requested by player.
 */
struct PlayerInputFlags {
	bool left: 1;
	bool up: 1;
	bool right: 1;
	bool down: 1;
	bool deflate: 1;

	PlayerInputFlags()
		: left{false}
		, up{false}
		, right{false}
		, down{false}
		, deflate{false}
	{}

	/**
	 * @brief Modify `this` by merging the actions of `this` and `rhs`.
	 * 
	 * @return Reference to `this`.
	 */
	PlayerInputFlags& operator|= (const PlayerInputFlags& rhs) {
		left = left || rhs.left;
		up = up || rhs.up;
		right = right || rhs.right;
		down = down || rhs.down;
		deflate = deflate || rhs.deflate;
		return *this;
	}
	/**
	 * @brief Merge the actions of `this` and `rhs`.
	 */
	PlayerInputFlags operator| (const PlayerInputFlags& rhs) const {
		PlayerInputFlags res = *this;
		res |= rhs;
		return res;
	}

	bool operator== (const PlayerInputFlags& rhs) const {
		return (this->left    == rhs.left)
			&& (this->up      == rhs.up)
			&& (this->right   == rhs.right)
			&& (this->down    == rhs.down)
			&& (this->deflate == rhs.deflate);
	}
	bool operator!= (const PlayerInputFlags& rhs) const {
		return !(*this == rhs);
	}

	/**
	 * @brief Create input flags representing movement to the left (West).
	 */
	static PlayerInputFlags createW() {
		PlayerInputFlags res;
		res.left = true;
		return res;
	}
	/**
	 * @brief Create input flags representing movement upwards (North).
	 */
	static PlayerInputFlags createN() {
		PlayerInputFlags res;
		res.up = true;
		return res;
	}
	/**
	 * @brief Create input flags representing movement to the right (East).
	 */
	static PlayerInputFlags createE() {
		PlayerInputFlags res;
		res.right = true;
		return res;
	}
	/**
	 * @brief Create input flags representing movement downwards (South).
	 */
	static PlayerInputFlags createS() {
		PlayerInputFlags res;
		res.down = true;
		return res;
	}
	/**
	 * @brief Create input flags representing deflate action.
	 */
	static PlayerInputFlags createDeflate() {
		PlayerInputFlags res;
		res.deflate = true;
		return res;
	}
	/**
	 * @brief Create input flags representing "northwest" movement.
	 */
	static PlayerInputFlags createNW() {
		return createN() | createW();
	}
	/**
	 * @brief Create input flags representing "northeast" movement.
	 */
	static PlayerInputFlags createNE() {
		return createN() | createE();
	}
	/**
	 * @brief Create input flags representing "southeast" movement.
	 */
	static PlayerInputFlags createSE() {
		return createS() | createE();
	}
	/**
	 * @brief Create input flags representing "southwest" movement.
	 */
	static PlayerInputFlags createSW() {
		return createS() | createW();
	}

	/**
	 * @brief Convert player input value to a vector.
	 * 
	 * @details Based on the player input creates a unit vector in the direction
	 *          the player wants to move.
	 * 
	 * @param x Vector X coordinate.
	 * @param y Vector Y coordinate.
	 */
	void toVector(double& x, double& y) const {
		// If none or both keys for horizontal movement are pressed, the player
		// won't move horizontally
		bool isHorzMove = (left != right);
		// Same as isHorzMove
		bool isVertMove = (up != down);

		if (!isHorzMove && !isVertMove) {
			// Doesn't move

			x = 0.0;
			y = 0.0;
		} else if (isHorzMove && isVertMove) {
			// Diagonal

			x = (left ? -DXY_DIAGONAL : DXY_DIAGONAL);
			y = (up ? -DXY_DIAGONAL : DXY_DIAGONAL);
		} else {
			// Orthogonal
			
			if (isHorzMove) {
				x = (left ? -1.0 : 1.0);
				y = 0.0;
			} else {
				x = 0.0;
				y = (up ? -1.0 : 1.0);
			}
		}
	}
};

#endif // PLAYERINPUTFLAGS_HPP
