/**
 * @file ControllerFactory.hpp
 * @author Tomáš Ludrovan
 * @brief ControllerFactory class
 * @version 0.1
 * @date 2023-12-18
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef CONTROLLERFACTORY_HPP
#define CONTROLLERFACTORY_HPP

#include <memory>

#include "controller/IRootController.hpp"
#include "controller/IControllerChild.hpp"
#include "gamesetupdata/GameSetupData.hpp"
#include "playerinput/IPlayerInput.hpp"
#include "stageeditor/StageEditor.hpp"
#include "stageserializer/IStageSerializer.hpp"
#include "sysproxy/ISysProxy.hpp"

/**
 * @brief Factory for the IRootController and IControllerChild interfaces.
 */
class ControllerFactory {
public:
	/**
	 * @brief Constructs a new RootController object.
	 * 
	 * @param sysProxy Proxy for communication with the system manager
	 *                 (dependency injection). 
	 */
	static std::shared_ptr<IRootController> createRootController(std::shared_ptr<ISysProxy> sysProxy);
	/**
	 * @brief Constructs a new InitialController object.
	 * 
	 * @param sysProxy Proxy for communication with the system manager
	 *                 (dependency injection). 
	 */
	static std::shared_ptr<IControllerChild> createInitialController(std::shared_ptr<ISysProxy> sysProxy);
	/**
	 * @brief Constructs a new LogoController object.
	 * 
	 * @param sysProxy Proxy for communication with the system manager
	 *                 (dependency injection). 
	 */
	static std::shared_ptr<IControllerChild> createLogoController(std::shared_ptr<ISysProxy> sysProxy);
	/**
	 * @brief Constructs a new MainMenuController object.
	 * 
	 * @param sysProxy Proxy for communication with the system manager
	 *                 (dependency injection). 
	 */
	static std::shared_ptr<IControllerChild> createMainMenuController(std::shared_ptr<ISysProxy> sysProxy);
	/**
	 * @brief Constructs a new InGameController object.
	 * 
	 * @param sysProxy Proxy for communication with the system manager
	 *                 (dependency injection). 
	 */
	static std::shared_ptr<IControllerChild> createInGameController(
		std::shared_ptr<ISysProxy> sysProxy, const GameSetupData& gsdata);
	/**
	 * @brief Constructs a new StageEditorController object.
	 * 
	 * @param sysProxy Proxy for communication with the system manager
	 *                 (dependency injection). 
	 */
	static std::shared_ptr<IControllerChild> createStageEditorController(std::shared_ptr<ISysProxy> sysProxy);
	static std::shared_ptr<IControllerChild> createStagePropertiesController(
		std::shared_ptr<ISysProxy> sysProxy,
		std::shared_ptr<StageEditor> stageEditor);
	static std::shared_ptr<IControllerChild> createStageSelectController(
		std::shared_ptr<ISysProxy> sysProxy,
		IStageSerializer::IdType& selectedStage, bool& isSelectedStageValid);
	static std::shared_ptr<IControllerChild> createGameSetupController(
		std::shared_ptr<ISysProxy> sysProxy, GameSetupData& gsdata,
		bool& isCancel);
};

#endif // CONTROLLERFACTORY_HPP
