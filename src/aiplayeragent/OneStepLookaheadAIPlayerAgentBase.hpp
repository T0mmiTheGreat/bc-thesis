/**
 * @file OneStepLookaheadAIPlayerAgentBase.hpp
 * @author Tomáš Ludrovan
 * @brief OneStepLookaheadAIPlayerAgentBase class
 * @version 0.1
 * @date 2024-03-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef ONESTEPLOOKAHEADAIPLAYERAGENTBASE_HPP
#define ONESTEPLOOKAHEADAIPLAYERAGENTBASE_HPP

#include <vector>

#include "aiplayeragent/AIPlayerAgentBase.hpp"
#include "playerinput/PlayerInputFlags.hpp"

class OneStepLookaheadAIPlayerAgentBase : public AIPlayerAgentBase {
private:
	static std::vector<PlayerInputFlags> generateInputs();
	double evaluatePosition(const Point_2& pos) const;
protected:
	PlayerInputFlags playerInput;
	
	void doPlan() override;
	PlayerInputFlags doGetPlayerInput() override;

	virtual Point_2 calculateNewPosition(const PlayerInputFlags& input)
		const = 0;
	virtual double evaluatePlayer(
		const GameStateAgentProxy::PlayerState& player, const Point_2& pos)
		const = 0;

	Point_2 calculateNewPositionBlind(const PlayerInputFlags& input) const;
	Point_2 calculateNewPositionWallAware(const PlayerInputFlags& input) const;
	double evaluatePlayerPredator(
		const GameStateAgentProxy::PlayerState& player, const Point_2& pos)
		const;
	double evaluatePlayerPrey(
		const GameStateAgentProxy::PlayerState& player, const Point_2& pos)
		const;
public:
	OneStepLookaheadAIPlayerAgentBase(PlayerId playerId);
};

#endif // ONESTEPLOOKAHEADAIPLAYERAGENTBASE_HPP
