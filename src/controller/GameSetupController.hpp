/**
 * @file GameSetupController.hpp
 * @author Tomáš Ludrovan
 * @brief GameSetupController class
 * @version 0.1
 * @date 2024-03-17
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef GAMESETUPCONTROLLER_HPP
#define GAMESETUPCONTROLLER_HPP

#include <array>
#include <memory>
#include <vector>

#include "types.hpp"
#include "controller/GeneralControllerBase.hpp"
#include "gamesetupdata/GameSetupData.hpp"
#include "playerinput/IPlayerInput.hpp"
#include "stageserializer/IStageSerializer.hpp"
#include "sysproxy/ISysProxy.hpp"

class GameSetupController : public GeneralControllerBase {
private:
	// We don't even have colors for more players than this
	static constexpr int MAX_PLAYERS = 8;

	enum PlayerSpecies {
		PLAYER_HUMAN,
		PLAYER_BOT,
		
		COUNT_PLAYERSPECIES,
	};
	enum PlayerInputType {
		INPUT_WSAD_Q,
		INPUT_IKJL_U,
		INPUT_ARROWS_SHIFT,
		
		COUNT_PLAYERINPUTTYPE,
	};
	enum PlayerBrainType {
		BRAIN_LADYBUG,
		BRAIN_BLIND_PREDATOR,
		BRAIN_BLIND_PREY,
		BRAIN_WALL_AWARE_PREDATOR,
		BRAIN_WALL_AWARE_PREY,

		COUNT_PLAYERBRAINTYPE,
	};

	struct PlayerDataInternal {
		PlayerSpecies species;
		PlayerInputType humanInput;
		PlayerBrainType botBrain;
	};
	
	struct GameSetupDataInternal {
		std::shared_ptr<IStageSerializer> stage;
		bool isStageIdValid;
		std::array<PlayerDataInternal, MAX_PLAYERS> players;
		int playerCount;
	};

	enum ExitResult {
		RES_STAGE_SELECT,
		RES_RETURN,
	};

	enum ClickedBtn {
		BTNCL_NONE,
		BTNCL_STAGE_SELECT,
		BTNCL_OK,
		BTNCL_CANCEL,
	};
private:
	ExitResult m_exitResult;

	GameSetupData& m_gsdata;
	bool& m_isConfirmSetup;
	GameSetupDataInternal m_gsdataInternal;

	ClickedBtn m_clickedBtn;

	IStageSerializer::IdType m_loadedStageId;
	bool m_isLoadedStageValid;

	/**
	 * @brief Updates `m_gsdata` by `m_gsdataInternal` value.
	 */
	void updateGsdata();

	void imguiWindow();
	void imguiWindowContent();

	bool canConfirmSetup() const;

	static const char* playerSpeciesToText(PlayerSpecies species);
	static const char* playerInputToText(PlayerInputType input);
	static const char* playerBrainToText(PlayerBrainType brain);
	static const char* playerInputOrBrainToText(
		const PlayerDataInternal& playerData);
	
	std::shared_ptr<IPlayerInput> playerDataToPlayerInput(
		const PlayerDataInternal& playerData, PlayerId playerId,
		std::shared_ptr<IAIPlayerAgent>& botAgent) const;
	std::shared_ptr<IPlayerInput> playerHumanToPlayerInput(
		PlayerInputType input, PlayerId playerId,
		std::shared_ptr<IAIPlayerAgent>& botAgent) const;
	std::shared_ptr<IPlayerInput> playerBotToPlayerInput(
		PlayerBrainType brain, PlayerId playerId,
		std::shared_ptr<IAIPlayerAgent>& botAgent) const;
protected:
	std::shared_ptr<IControllerChild> createReplacement() override;
	
	void onStarted() override;
	void onResumed() override;
	void onLoop() override;
	void onPaint(std::shared_ptr<ICanvas> canvas,
		const Rect& invalidRect) override;
public:
	GameSetupController(std::shared_ptr<ISysProxy> sysProxy,
		GameSetupData& gsdata, bool& isConfirmSetup);
};

#endif // GAMESETUPCONTROLLER_HPP
