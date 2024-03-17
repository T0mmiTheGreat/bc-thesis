#
# file: CMakeLists.txt
# author: Tomáš Ludrovan
# version: 0.1
# date: 2023-11-14
#
cmake_minimum_required(VERSION 3.25)

project(BUBLRAWL LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

cmake_policy(SET CMP0153 OLD)

include_directories(. ../imgui/)

# Preprocessor macros

# The old algorithm for computing player movement trajectory among obstacles.
# It turned out to be too slow.
#add_compile_definitions(OLD_TRAJECTORY_ALGORITHM)


# Sources (*.cpp)
set(SOURCE_FILES
	main.cpp
	functions.cpp
	SDL2_gfxPrimitives.cpp
	sdlmanager/SDLManager.cpp
	sdlsubscriber/SDLSubscriber.cpp
	sysproxy/SysProxyBase.cpp
	sysproxy/SDLSysProxy.cpp
	sysproxy/SysProxyFactory.cpp
	canvas/CanvasBase.cpp
	canvas/SDLCanvas.cpp
	canvas/CanvasFactory.cpp
	controller/ControllerBase.cpp
	controller/ControllerFactory.cpp
	controller/GeneralControllerBase.cpp
	controller/RootController.cpp
	controller/InitialController.cpp
	controller/LogoController.cpp
	controller/MainMenuController.cpp
	controller/InGameController.cpp
	controller/StageEditorController.cpp
	controller/StagePropertiesController.cpp
	controller/StageSelectController.cpp
	controller/GameSetupController.cpp
	math/Math.cpp
	sprite/SpriteBase.cpp
	sprite/BoundedSpriteBase.cpp
	sprite/PositionedSpriteBase.cpp
	sprite/AnimatedSpriteBase.cpp
	sprite/LogoSprite.cpp
	sprite/OptionButtonSprite.cpp
	sprite/MainMenuItemSprite.cpp
	sprite/MainMenuTitleSprite.cpp
	sprite/PlayerSprite.cpp
	sprite/ObstacleSprite.cpp
	sprite/BonusSprite.cpp
	sprite/ObstacleEdgesSprite.cpp
	sprite/ImageSprite.cpp
	sprite/EditorIconSprite.cpp
	sprite/OptionBarSprite.cpp
	sprite/StatusbarTextSprite.cpp
	sprite/EditorWorkspaceGridSprite.cpp
	sprite/PlayerBrushSprite.cpp
	sprite/ObstacleBrushSprite.cpp
	sprite/TextBoxSprite.cpp
	sprite/TextSprite.cpp
	sprite/StageItemSprite.cpp
	sprite/HollowRectSprite.cpp
	sprite/BonusHpRecoverySprite.cpp
	core/Core.cpp
	core/CoreAction.cpp
	core/stageobstacles/StageObstacles.cpp
	core/stagebonuses/StageBonuses.cpp
	core/trajectory/Trajectory.cpp
	core/playerstate/PlayerState.cpp
	core/bonuseffect/BonusEffect.cpp
	core/bonuseffect/EffectAttributes.cpp
	playerinput/PlayerInputFactory.cpp
	playerinput/PlayerInputBase.cpp
	playerinput/ImmobilePlayerInput.cpp
	playerinput/KeyboardPlayerInput.cpp
	playerinput/AIPlayerInput.cpp
	stageeditor/StageEditor.cpp
	stageeditor/StageEditorObjects.cpp
	stageeditor/StageState.cpp
	stageeditor/StageEditorAction.cpp
	stageeditor/StageEditorHistory.cpp
	stageviewport/StageViewport.cpp
	stageserializer/StageSerializerBase.cpp
	stageserializer/StageSerializerFactory.cpp
	stageserializer/YAMLStageSerializer.cpp
)

# Headers (*.hpp)
set(HEADER_FILES
	SDL2_gfxPrimitives.h
	constants.hpp
	types.hpp
	functions.hpp
	sdlmanager/SDLManager.hpp
	sdlsubscriber/ISDLSubscriber.hpp
	sdlsubscriber/SDLSubscriber.hpp
	syssubscriber/ISysSubscriber.hpp
	sysproxy/ISysProxy.hpp
	sysproxy/SysProxyBase.hpp
	sysproxy/SDLSysProxy.hpp
	sysproxy/SysProxyFactory.hpp
	canvas/ICanvas.hpp
	canvas/CanvasBase.hpp
	canvas/SDLCanvas.hpp
	canvas/CanvasFactory.hpp
	controller/IControllerEventSubscriber.hpp
	controller/IControllerChild.hpp
	controller/IRootController.hpp
	controller/ControllerBase.hpp
	controller/ControllerFactory.hpp
	controller/GeneralControllerBase.hpp
	controller/RootController.hpp
	controller/InitialController.hpp
	controller/LogoController.hpp
	controller/MainMenuController.hpp
	controller/InGameController.hpp
	controller/StageEditorController.hpp
	controller/StagePropertiesController.hpp
	controller/StageSelectController.hpp
	controller/GameSetupController.hpp
	math/Math.hpp
	sprite/ISprite.hpp
	sprite/IPositionedSprite.hpp
	sprite/IAnimatedSprite.hpp
	sprite/SpriteBase.hpp
	sprite/BoundedSpriteBase.hpp
	sprite/PositionedSpriteBase.hpp
	sprite/AnimatedSpriteBase.hpp
	sprite/LogoSprite.hpp
	sprite/OptionButtonSprite.hpp
	sprite/MainMenuItemSprite.hpp
	sprite/MainMenuTitleSprite.hpp
	sprite/PlayerSprite.hpp
	sprite/ObstacleSprite.hpp
	sprite/BonusSprite.hpp
	sprite/ObstacleEdgesSprite.hpp
	sprite/ImageSprite.hpp
	sprite/EditorIconSprite.hpp
	sprite/OptionBarSprite.hpp
	sprite/StatusbarTextSprite.hpp
	sprite/EditorWorkspaceGridSprite.hpp
	sprite/PlayerBrushSprite.hpp
	sprite/ObstacleBrushSprite.hpp
	sprite/TextBoxSprite.hpp
	sprite/TextSprite.hpp
	sprite/StageItemSprite.hpp
	sprite/HollowRectSprite.hpp
	sprite/BonusHpRecoverySprite.hpp
	paintingproxy/IPaintingProxy.hpp
	core/Common.hpp
	core/Core.hpp
	core/CoreAction.hpp
	core/stageobstacles/StageObstacles.hpp
	core/stagebonuses/StageBonuses.hpp
	core/trajectory/Trajectory.hpp
	core/playerstate/PlayerState.hpp
	core/geometry/Geometry.hpp
	core/bonuseffect/BonusEffect.hpp
	core/bonuseffect/EffectAttributes.hpp
	playerinput/IPlayerInput.hpp
	playerinput/PlayerInputFactory.hpp
	playerinput/PlayerInputBase.hpp
	playerinput/KeyboardPlayerInput.hpp
	playerinput/ImmobilePlayerInput.hpp
	playerinput/AIPlayerInput.hpp
	playerinput/ISysProxyPlayerInput.hpp
	stageeditor/Common.hpp
	stageeditor/StageEditor.hpp
	stageeditor/StageEditorObjects.hpp
	stageeditor/StageState.hpp
	stageeditor/StageEditorAction.hpp
	stageeditor/StageEditorHistory.hpp
	stageviewport/StageViewport.hpp
	stageserializer/IStageSerializer.hpp
	stageserializer/StageSerializerBase.hpp
	stageserializer/StageSerializerFactory.hpp
	stageserializer/YAMLStageSerializer.hpp
	utilities/unorderedSetWithIndexes/UnorderedSetWithIndexes.hpp
	gamesetupdata/GameSetupData.hpp
)

# Imgui files
file (GLOB IMGUI_SOURCES
	../imgui/*.cpp ../imgui/*.h
	../imgui/backends/imgui_impl_sdl2.h ../imgui/backends/imgui_impl_sdl2.cpp
	../imgui/backends/imgui_impl_sdlrenderer2.h ../imgui/backends/imgui_impl_sdlrenderer2.cpp
)

# Create the executable in the repo root directory
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../)

# Setup libSDL2pp
set(SDL2PP_WITH_TTF ON)
set(SDL2PP_WITH_IMAGE ON)
set(SDL2PP_WITH_MIXER OFF)
add_subdirectory(../libSDL2pp/ ../libSDL2pp/)

# Include packages
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
find_package(SDL2_ttf REQUIRED)
include_directories(${SDL2_ttf_INCLUDE_DIRS})
find_package(SDL2_image REQUIRED)
include_directories(${SDL2_image_INCLUDE_DIRS})
set(CGAL_DO_NOT_WARN_ABOUT_CMAKE_BUILD_TYPE TRUE)
find_package(CGAL REQUIRED)
include_directories(${CGAL_INCLUDE_DIRS})
find_package(YAML-CPP REQUIRED)
include_directories(${YAML_CPP_INCLUDE_DIR})

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES} ${IMGUI_SOURCES})

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)

# Link libraries
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${SDL2_ttf_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${SDL2_image_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${CGAL_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${YAML_CPP_LIBRARIES})
target_link_libraries(${PROJECT_NAME} SDL2pp::SDL2pp)
