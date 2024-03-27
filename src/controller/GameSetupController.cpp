/**
 * @file GameSetupController.cpp
 * @author Tomáš Ludrovan
 * @brief GameSetupController class
 * @version 0.1
 * @date 2024-03-17
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "controller/GameSetupController.hpp"

#include "imgui.h"

#include "aiplayeragent/AIPlayerAgentFactory.hpp"
#include "controller/ControllerFactory.hpp"
#include "playerinput/PlayerInputFactory.hpp"
#include "stageserializer/StageSerializerFactory.hpp"

void GameSetupController::updateGsdata()
{
	assert(m_gsdataInternal.isStageIdValid);

	m_gsdata.stage = m_gsdataInternal.stage;

	std::shared_ptr<IAIPlayerAgent> botAgent;

	m_gsdata.players.clear();
	m_gsdata.aiAgents.clear();
	for (PlayerId i = 0; static_cast<int>(i) < m_gsdataInternal.playerCount;
		++i)
	{
		m_gsdata.players.push_back(playerDataToPlayerInput(
			m_gsdataInternal.players[i], i, botAgent));
		
		if (botAgent != nullptr) {
			m_gsdata.aiAgents.push_back(std::move(botAgent));
		}
	}
}

void GameSetupController::imguiWindow()
{
	// Begin invisible window
	ImGuiWindowFlags flags
		= ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoBackground;
	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
	ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
	ImGui::Begin("``Window'' is a windy town in Poland", NULL, flags);

	// Add content
	imguiWindowContent();

	// End window
	ImGui::End();
}

void GameSetupController::imguiWindowContent()
{
	int maxStagePlayers = (m_gsdataInternal.isStageIdValid 
		? m_gsdataInternal.stage->getPlayers().size()
		: 0
	);
	if (maxStagePlayers > MAX_PLAYERS) maxStagePlayers = MAX_PLAYERS;

	// Stage selection
	ImGui::Text("Stage:");
	std::string stageBtnLabel(m_gsdataInternal.isStageIdValid
		? m_gsdataInternal.stage->getTitle()
		: "-- Select stage --");
	stageBtnLabel += "###stage_select";
	ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0, 0.5));
	if (ImGui::Button(stageBtnLabel.c_str(), ImVec2(-50.0, 0))) {
		m_clickedBtn = BTNCL_STAGE_SELECT;
	}
	ImGui::PopStyleVar();

	ImGui::Separator();

	// Player count selection
	ImGui::Text("Players:");
	ImGui::InputInt("##player_count", &m_gsdataInternal.playerCount);
	if (m_gsdataInternal.playerCount < 0)
		m_gsdataInternal.playerCount = 0;
	if (m_gsdataInternal.playerCount > maxStagePlayers)
		m_gsdataInternal.playerCount = maxStagePlayers;

	// Player data
	if (ImGui::BeginTable("table1", 2, ImGuiTableFlags_Borders)) {
		for (int playerIdx = 0; playerIdx < m_gsdataInternal.playerCount;
			++playerIdx)
		{
			// Alias
			auto& playerRef = m_gsdataInternal.players[playerIdx];

			// Create table cell
			ImGui::TableNextColumn();

			ImGui::PushID(playerIdx);

			// Player species
			int* speciesPtr = (int*)(&playerRef.species);
			ImGui::RadioButton("Human", speciesPtr, 0);
			ImGui::RadioButton("Bot", speciesPtr, 1);

			// Input/Brain
			switch (playerRef.species) {
				case PLAYER_HUMAN: {
					// Input

					const char* items[COUNT_PLAYERINPUTTYPE];
					for (int i = 0; i < COUNT_PLAYERINPUTTYPE; ++i) {
						items[i] = playerInputToText((PlayerInputType)i);
					}
					ImGui::Combo("##", (int*)&playerRef.humanInput, items,
						(int)COUNT_PLAYERINPUTTYPE);
				} break;
				case PLAYER_BOT: {
					// Brain
					
					const char* items[COUNT_PLAYERBRAINTYPE];
					for (int i = 0; i < COUNT_PLAYERBRAINTYPE; ++i) {
						items[i] = playerBrainToText((PlayerBrainType)i);
					}
					ImGui::Combo("##", (int*)&playerRef.botBrain, items,
						(int)COUNT_PLAYERBRAINTYPE);
				} break;
				case COUNT_PLAYERSPECIES: break;
			}

			ImGui::PopID();
		}

		ImGui::EndTable();
	}

	bool isOkBtnDisabled = !canConfirmSetup();

	// Dialog buttons
	ImGui::BeginDisabled(isOkBtnDisabled);
	if (ImGui::Button("OK")) {
		m_clickedBtn = BTNCL_OK;
	}
	ImGui::EndDisabled();
	ImGui::SameLine();
	if (ImGui::Button("Cancel")) {
		m_clickedBtn = BTNCL_CANCEL;
	}
}

bool GameSetupController::canConfirmSetup() const
{
	if (!m_gsdataInternal.isStageIdValid) return false;

	// Find duplicate inputs
	for (int i = 0; i < m_gsdataInternal.playerCount; ++i) {
		// Alias
		auto& dataRefI = m_gsdataInternal.players[i];

		if (dataRefI.species != PLAYER_HUMAN) continue;

		for (int j = i + 1; j < m_gsdataInternal.playerCount; ++j) {
			// Alias
			auto& dataRefJ = m_gsdataInternal.players[j];

			if (dataRefJ.species != PLAYER_HUMAN) continue;

			if (dataRefI.humanInput == dataRefJ.humanInput) {
				return false;
			}
		}
	}

	return true;
}

const char* GameSetupController::playerSpeciesToText(PlayerSpecies species)
{
	switch (species) {
		case PLAYER_HUMAN: return "Human";
		case PLAYER_BOT:   return "Bot";
		case COUNT_PLAYERSPECIES: break;
	}

	// Default
	assert(((void)"Invalid player species", false));
	return nullptr;
}

const char* GameSetupController::playerInputToText(PlayerInputType input)
{
	switch (input) {
		case INPUT_WSAD_Q:       return "WSAD + Q";
		case INPUT_IKJL_U:       return "IKJL + U";
		case INPUT_ARROWS_SHIFT: return "Arrow keys + Shift";
		case COUNT_PLAYERINPUTTYPE: break;
	}

	// Default
	assert(((void)"Invalid player input", false));
	return nullptr;
}

const char* GameSetupController::playerBrainToText(PlayerBrainType brain)
{
	switch (brain) {
		case BRAIN_LADYBUG: return "Ladybug";
		case BRAIN_BLIND_PREDATOR: return "Blind predator";
		case COUNT_PLAYERBRAINTYPE: break;
	}

	// Default
	assert(((void)"Invalid player brain", false));
	return nullptr;
}

const char* GameSetupController::playerInputOrBrainToText(
	const PlayerDataInternal& playerData)
{
	switch (playerData.species) {
		case PLAYER_HUMAN: return playerInputToText(playerData.humanInput);
		case PLAYER_BOT:   return playerBrainToText(playerData.botBrain);
		case COUNT_PLAYERSPECIES: break;
	}

	// Default
	assert(((void)"Invalid player species", false));
	return nullptr;
}

std::shared_ptr<IPlayerInput> GameSetupController::playerDataToPlayerInput(
	const PlayerDataInternal& playerData, PlayerId playerId,
	std::shared_ptr<IAIPlayerAgent>& botAgent) const
{
	switch (playerData.species) {
		case PLAYER_HUMAN:
			return playerHumanToPlayerInput(playerData.humanInput, playerId,
				botAgent);
		case PLAYER_BOT:
			return playerBotToPlayerInput(playerData.botBrain, playerId,
				botAgent);
		case COUNT_PLAYERSPECIES: break;
	}

	// Default
	assert(((void)"Invalid player species", false));
	return nullptr;
}

std::shared_ptr<IPlayerInput> GameSetupController::playerHumanToPlayerInput(
	PlayerInputType input, PlayerId playerId,
	std::shared_ptr<IAIPlayerAgent>& botAgent) const
{
	(void)playerId;
	botAgent = nullptr;

	switch (input) {
		case INPUT_WSAD_Q:
			return PlayerInputFactory::createKeyboardPlayerInputWSADQ(sysProxy);
		case INPUT_IKJL_U:
			return PlayerInputFactory::createKeyboardPlayerInputIKJLU(sysProxy);
		case INPUT_ARROWS_SHIFT:
			return PlayerInputFactory::createKeyboardPlayerInputArrowsShift(
				sysProxy);
		case COUNT_PLAYERINPUTTYPE: break;
	}

	// Default
	assert(((void)"Invalid player input", false));
	return nullptr;
}

std::shared_ptr<IPlayerInput> GameSetupController::playerBotToPlayerInput(
	PlayerBrainType brain, PlayerId playerId,
	std::shared_ptr<IAIPlayerAgent>& botAgent) const
{
	switch (brain) {
		case BRAIN_LADYBUG:
			botAgent = AIPlayerAgentFactory::createLadybugAIPlayerAgent(
				playerId);
			return PlayerInputFactory::createAIPlayerInput(botAgent);
		case BRAIN_BLIND_PREDATOR:
			botAgent = AIPlayerAgentFactory::createBlindPredatorAIPlayerAgent(
				playerId);
			return PlayerInputFactory::createAIPlayerInput(botAgent);
		case COUNT_PLAYERBRAINTYPE: break;
	}

	// Default
	assert(((void)"Invalid player brain", false));
	return nullptr;
}

std::shared_ptr<IControllerChild> GameSetupController::createReplacement()
{
	switch (m_exitResult) {
		case RES_STAGE_SELECT:
			return ControllerFactory::createStageSelectController(sysProxy,
				m_loadedStageId, m_isLoadedStageValid);
		case RES_RETURN:
			return nullptr;
	}

	// Default
	assert(((void)"Invalid ExitResult value", false));
	return nullptr;
}

void GameSetupController::onStarted()
{
	GeneralControllerBase::onStarted();
}

void GameSetupController::onResumed()
{
	if (m_exitResult == RES_STAGE_SELECT) {
		if (m_isLoadedStageValid) {
			m_gsdataInternal.stage->load(m_loadedStageId);
			m_gsdataInternal.isStageIdValid = true;
		}
	}
}

void GameSetupController::onLoop()
{
	// Although the value would had been activated during the previous loop,
	// we had to not check the value until now. Pause/finished events would
	// deactivate this controller, but repainting ImGui would not be stopped.
	if (m_clickedBtn != BTNCL_NONE) {
		switch (m_clickedBtn) {
			case BTNCL_STAGE_SELECT:
				m_exitResult = RES_STAGE_SELECT;
				pausedEvent();
				break;
			case BTNCL_OK:
				updateGsdata();
				m_isConfirmSetup = true;
				m_exitResult = RES_RETURN;
				finishedEvent();
				break;
			case BTNCL_CANCEL:
				m_isConfirmSetup = false;
				m_exitResult = RES_RETURN;
				finishedEvent();
				break;
			case BTNCL_NONE: break;
		}
		m_clickedBtn = BTNCL_NONE;
	} else {
		imguiWindow();

		sysProxy->invalidateRect();
	}
}

void GameSetupController::onPaint(std::shared_ptr<ICanvas> canvas,
	const Rect& invalidRect)
{
	GeneralControllerBase::onPaint(canvas, invalidRect);
}

GameSetupController::GameSetupController(std::shared_ptr<ISysProxy> sysProxy,
	GameSetupData& gsdata, bool& isConfirmSetup)
	: GeneralControllerBase(sysProxy)
	, m_gsdata{gsdata}
	, m_isConfirmSetup{isConfirmSetup}
	, m_gsdataInternal{
		StageSerializerFactory::createDefault(), // stage
		false, // isStageIdValid
		std::array<PlayerDataInternal, MAX_PLAYERS>(), // players
		0, // playerCount
	}
	, m_clickedBtn{BTNCL_NONE}
	, m_loadedStageId()
	, m_isLoadedStageValid{false}
{}
