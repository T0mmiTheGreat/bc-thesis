#
# file: CMakeLists.txt
# author: Tomáš Ludrovan
# version: 0.1
# date: 2023-11-14
#
cmake_minimum_required(VERSION 3.25)

project(krpole LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

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
	sprite/SpriteBase.cpp
	sprite/LogoSprite.cpp
	sprite/MainMenuItemSprite.cpp
	sprite/MainMenuTitleSprite.cpp
	sprite/MainMenuSprite.cpp
	core/Core.cpp
	playerinput/PlayerInputFactory.cpp
	playerinput/PlayerInputBase.cpp
	playerinput/ImmobilePlayerInput.cpp
	playerinput/KeyboardPlayerInput.cpp
	playerstate/PlayerStateFactory.cpp
	playerstate/PlayerStateBase.cpp
	playerstate/DefaultPlayerState.cpp
)

# Headers (*.hpp)
set(HEADER_FILES
	SDL2_gfxPrimitives.h
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
	sprite/ISprite.hpp
	sprite/SpriteBase.hpp
	sprite/LogoSprite.hpp
	sprite/MainMenuItemSprite.hpp
	sprite/MainMenuTitleSprite.hpp
	sprite/MainMenuSprite.hpp
	paintingproxy/IPaintingProxy.hpp
	core/ICore.hpp
	core/Core.hpp
	playerinput/IPlayerInput.hpp
	playerinput/PlayerInputFactory.hpp
	playerinput/PlayerInputBase.hpp
	playerinput/KeyboardPlayerInput.hpp
	playerinput/ImmobilePlayerInput.hpp
	playerstate/IPlayerState.hpp
	playerstate/PlayerStateFactory.hpp
	playerstate/PlayerStateBase.hpp
	playerstate/DefaultPlayerState.hpp
)

# Create the executable in the repo root directory
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../)

set(SDL2PP_WITH_TTF ON)
set(SDL2PP_WITH_IMAGE OFF)
set(SDL2PP_WITH_MIXER OFF)
add_subdirectory(../libSDL2pp/ ../libSDL2pp/)

# Include packages
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
find_package(SDL2_ttf REQUIRED)
include_directories(${SDL2_ttf_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

# Link libraries
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${SDL2_ttf_LIBRARIES})
target_link_libraries(${PROJECT_NAME} SDL2pp::SDL2pp)
