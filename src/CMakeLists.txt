#
# file: CMakeLists.txt
# author: Tomáš Ludrovan
# version: 0.1
# date: 2023-11-14
#
cmake_minimum_required(VERSION 3.25)

project(krpole LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

# Sources (*.cpp)
set(SOURCE_FILES
	main.cpp
	scene.cpp
	sprite.cpp
	SDL2_gfxPrimitives.cpp
	sysevent/SysEventPublisherBase.cpp
	sysevent/SDLSysEventPublisher.cpp
)

# Headers (*.h)
set(HEADER_FILES
	scene.h
	sprite.h
	SDL2_gfxPrimitives.h
	sysevent/ISysEventPublisher.hpp
	sysevent/ISysEventSubscriber.hpp
	sysevent/SysEventPublisherBase.hpp
	sysevent/SDLSysEventPublisher.hpp
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
