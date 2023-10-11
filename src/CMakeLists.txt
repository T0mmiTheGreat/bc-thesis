cmake_minimum_required(VERSION 3.25)

project(krpole LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES
	main.cpp
)

set(HEADER_FILES )

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../../)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(${PROJECT_NAME}  ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
