/**
 * @file IPlayerState.hpp
 * @author Tomáš Ludrovan
 * @brief IPlayerState interface
 * @version 0.1
 * @date 2023-12-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef IPLAYERSTATE_HPP
#define IPLAYERSTATE_HPP

#include <memory>

#include "playerinput/IPlayerInput.hpp"

class IPlayerState {
public:
	virtual ~IPlayerState() {}
	virtual double getX() = 0;
	virtual void setX(double value) = 0;
	virtual double getY() = 0;
	virtual void setY(double value) = 0;
	virtual double getHp() = 0;
	virtual void setHp(double value) = 0;
	virtual double getSize() = 0;
	virtual double getSpeed() = 0;
	virtual std::shared_ptr<IPlayerInput> getPlayerInputObject() = 0;
	virtual void setPlayerInputObject(std::shared_ptr<IPlayerInput> value) = 0;
};

#endif // IPLAYERSTATE_HPP
