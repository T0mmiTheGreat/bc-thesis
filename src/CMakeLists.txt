#
# file: CMakeLists.txt
# author: Tomáš Ludrovan
# version: 0.1
# date: 2023-11-14
#
cmake_minimum_required(VERSION 3.25)

project(krpole LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

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
)

# Headers (*.h)
set(HEADER_FILES
	SDL2_gfxPrimitives.h
	sdlmanager/SDLManager.hpp
	sdlsubscriber/ISDLSubscriber.hpp
	sdlsubscriber/SDLSubscriber.hpp
	syssubscriber/ISysSubscriber.hpp
	sysproxy/ISysProxy.hpp
	sysproxy/SysProxyBase.hpp
	sysproxy/SDLSysProxy.hpp
	types.hpp
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
)

# Create the executable in the repo root directory
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../../)

set(SDL2PP_WITH_IMAGE OFF)
set(SDL2PP_WITH_MIXER OFF)
set(SDL2PP_WITH_TTF OFF)
add_subdirectory(../libSDL2pp/ ../libSDL2pp/)

# Include packages
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

# Link libraries
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
target_link_libraries(${PROJECT_NAME} SDL2pp::SDL2pp)
