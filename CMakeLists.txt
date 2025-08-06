cmake_minimum_required(VERSION 3.12)

project(cube)
set(CMAKE_CXX_STANDARD 23)

include_directories(packages/include)
find_package(SDL3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
set(CUBE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})
add_subdirectory(packages)
add_subdirectory(projects)