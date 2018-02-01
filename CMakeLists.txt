cmake_minimum_required(VERSION 3.0.0)
project(LocalMonitor)

add_compile_options("-Wall")

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(PROJECT_INPUT_DIR ${PROJECT_SOURCE_DIR}/Input)
set(PROJECT_OUTPUT_DIR ${PROJECT_SOURCE_DIR}/Output)
set(PROJECT_BINARY_DIR ${PROJECT_SOURCE_DIR}/build)
set(LOCAL_MONITOR_BASE_DIR ${PROJECT_SOURCE_DIR})

#Find<package>.cmake files are Under directory 'cmake' 
list(APPEND CMAKE_MODULE_PATH ${LOCAL_MONITOR_BASE_DIR}/cmake)

set(nlohmann_json_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/Utility/json-2.1.1/src)

option(USE_GRAPHICS "Use graphics (Qt must be installed)" ON)

add_subdirectory(Observer)
add_subdirectory(Utility)
add_subdirectory(Basic) 	
add_subdirectory(Automation)
add_subdirectory(Input)
add_subdirectory(Simulator)
add_subdirectory(Tests)
