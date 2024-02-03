#
# file: CMakeLists.txt
# author: Tomáš Ludrovan
# version: 0.1
# date: 2023-11-14
#
cmake_minimum_required(VERSION 3.25)

project(krpole LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

cmake_policy(SET CMP0153 OLD)

include_directories(.)

# Sources (*.cpp)
set(SOURCE_FILES
	main.cpp
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
	math/Math.cpp
	sprite/SpriteBase.cpp
	sprite/BoundedSpriteBase.cpp
	sprite/PositionedSpriteBase.cpp
	sprite/AnimatedSpriteBase.cpp
	sprite/LogoSprite.cpp
	sprite/MainMenuItemSprite.cpp
	sprite/MainMenuTitleSprite.cpp
	sprite/PlayerSprite.cpp
	sprite/ObstacleSprite.cpp
	sprite/ObstacleEdgesSprite.cpp
	sprite/ImageSprite.cpp
	sprite/EditorIconSprite.cpp
	sprite/OptionBarSprite.cpp
	sprite/StatusbarTextSprite.cpp
	sprite/EditorWorkspaceGridSprite.cpp
	core/Core.cpp
	core/stageobstacles/StageObstacles.cpp
	core/trajectory/Trajectory.cpp
	playerinput/PlayerInputFactory.cpp
	playerinput/PlayerInputBase.cpp
	playerinput/ImmobilePlayerInput.cpp
	playerinput/KeyboardPlayerInput.cpp
	playerstate/PlayerStateFactory.cpp
	playerstate/PlayerStateBase.cpp
	playerstate/DefaultPlayerState.cpp
	stageeditor/StageEditor.cpp
	stageeditor/StageEditorObjects.cpp
	stageeditor/StageState.cpp
	stageeditor/StageViewport.cpp
	stageeditor/StageEditorAction.cpp
	stageeditor/StageEditorHistory.cpp
)

# Headers (*.hpp)
set(HEADER_FILES
	SDL2_gfxPrimitives.h
	constants.hpp
	types.hpp
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
	math/Math.hpp
	sprite/ISprite.hpp
	sprite/IPositionedSprite.hpp
	sprite/IAnimatedSprite.hpp
	sprite/SpriteBase.hpp
	sprite/BoundedSpriteBase.hpp
	sprite/PositionedSpriteBase.hpp
	sprite/AnimatedSpriteBase.hpp
	sprite/LogoSprite.hpp
	sprite/MainMenuItemSprite.hpp
	sprite/MainMenuTitleSprite.hpp
	sprite/PlayerSprite.hpp
	sprite/ObstacleSprite.hpp
	sprite/ObstacleEdgesSprite.hpp
	sprite/ImageSprite.hpp
	sprite/EditorIconSprite.hpp
	sprite/OptionBarSprite.hpp
	sprite/StatusbarTextSprite.hpp
	sprite/EditorWorkspaceGridSprite.hpp
	paintingproxy/IPaintingProxy.hpp
	core/ICore.hpp
	core/Core.hpp
	core/Constants.hpp
	core/stageobstacles/StageObstacles.cpp
	core/trajectory/Trajectory.hpp
	playerinput/IPlayerInput.hpp
	playerinput/PlayerInputFactory.hpp
	playerinput/PlayerInputBase.hpp
	playerinput/KeyboardPlayerInput.hpp
	playerinput/ImmobilePlayerInput.hpp
	playerinput/ISysProxyPlayerInput.hpp
	playerstate/IPlayerState.hpp
	playerstate/PlayerStateFactory.hpp
	playerstate/PlayerStateBase.hpp
	playerstate/DefaultPlayerState.hpp
	stageeditor/Common.hpp
	stageeditor/StageEditor.hpp
	stageeditor/StageEditorObjects.hpp
	stageeditor/StageState.hpp
	stageeditor/StageViewport.hpp
	stageeditor/StageEditorAction.hpp
	stageeditor/StageEditorHistory.hpp
)

# Create the executable in the repo root directory
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../)

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

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

# Link libraries
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${SDL2_ttf_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${SDL2_image_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${CGAL_LIBRARIES})
target_link_libraries(${PROJECT_NAME} SDL2pp::SDL2pp)
