/**
 * @file PlayerStateBase.hpp
 * @author Tomáš Ludrovan
 * @brief PlayerStateBase class
 * @version 0.1
 * @date 2023-12-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef PLAYERSTATEBASE_HPP
#define PLAYERSTATEBASE_HPP

#include <memory>

#include "playerstate/IPlayerState.hpp"

class PlayerStateBase : public IPlayerState {
protected:
	const double HP_FACTOR = 100.0;

	double x;
	double y;
	double hp;
	std::shared_ptr<IPlayerInput> playerInput;
public:
	PlayerStateBase(double startX, double startY, double startHp);
	virtual double getX() override;
	virtual void setX(double value) override;
	virtual double getY() override;
	virtual void setY(double value) override;
	virtual double getHp() override;
	virtual void setHp(double value) override;
	virtual std::shared_ptr<IPlayerInput> getPlayerInputObject() override;
	virtual void setPlayerInputObject(std::shared_ptr<IPlayerInput> value) override;
};

#endif // PLAYERSTATEBASE_HPP
