/**
 * @file ICore.hpp
 * @author Tomáš Ludrovan
 * @brief ICore interface
 * @version 0.1
 * @date 2023-12-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef ICORE_HPP
#define ICORE_HPP

#include <memory>
#include <vector>

#include "playerstate/IPlayerState.hpp"

// The type might change, but it will always meet the named requirement
// "Container" and the element type will be `std::shared_ptr<IPlayerState>`
typedef std::vector<std::shared_ptr<IPlayerState>> PlayerList;

class ICore {
public:
	virtual ~ICore() {}
	virtual void tick() = 0;
	virtual void addPlayer(std::shared_ptr<IPlayerState> player) = 0;
	virtual const PlayerList& getPlayerList() = 0;
};

#endif // ICORE_HPP
