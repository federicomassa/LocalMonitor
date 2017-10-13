cmake_minimum_required(VERSION 3.0.0)
project(LocalMonitor)

set(PROJECT_INPUT_DIR ${PROJECT_SOURCE_DIR}/Input)
set(PROJECT_OUTPUT_DIR ${PROJECT_SOURCE_DIR}/Output)
set(nlohmann_json_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/Utility/json-2.1.1/src)

add_subdirectory(Simulator)
add_subdirectory(Observer)
add_subdirectory(Utility)
add_subdirectory(Basic)
add_subdirectory(Automation)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR})
