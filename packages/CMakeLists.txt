cmake_minimum_required(VERSION 3.12)
project(cube)
file(GLOB_RECURSE SOURCES ./src/*.cpp)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CUBE_OUTPUT_PATH})
add_library(${PROJECT_NAME} SHARED ${SOURCES})

if(MINGW)
    target_link_libraries(${PROJECT_NAME} PRIVATE stdc++exp)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)
target_link_libraries(${PROJECT_NAME} PRIVATE glad::glad)
target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)
target_link_libraries(${PROJECT_NAME} PRIVATE cjson)