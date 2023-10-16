cmake_minimum_required(VERSION 3.25)

project(krpole LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

# Sources (*.cpp)
set(SOURCE_FILES
	main.cpp
)

# Headers (*.h)
set(HEADER_FILES )

# Create the executable in the repo root directory
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../../)

set(SDL2PP_WITH_IMAGE OFF)
set(SDL2PP_WITH_MIXER OFF)
set(SDL2PP_WITH_TTF OFF)
add_subdirectory(../libSDL2pp/ ../libSDL2pp/build/)

# Include packages
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

# Link libraries
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
target_link_libraries(${PROJECT_NAME} SDL2pp::SDL2pp)
