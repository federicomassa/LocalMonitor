cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_CXX_STANDARD 11)

set(SV_NAME
    SimulatorViewer)

set(SV_HEADERS_DIR
    include/${_NAME}
)

set(SV_SOURCE_DIR
    src
)

set(SV_SOURCES
	SimulatorViewer.cpp
	)
	
foreach(SOURCE IN ITEMS ${SV_SOURCES})
	set(SV_SOURCE ${SV_SOURCE} ${SV_SOURCE_DIR}/${SOURCE})
endforeach(SOURCE)

project(${SV_NAME})

add_library(${SV_NAME} STATIC
  ${SV_SOURCE}
  )
  
target_link_libraries(${SV_NAME} Utility)

# TODO: this could be unnecessary (only use necessary sources?)
target_link_libraries(${SV_NAME} Basic)
target_link_libraries(${SV_NAME} Automation)
target_include_directories(${SV_NAME} PRIVATE ${nlohmann_json_INCLUDE_DIRS})



target_include_directories(${SV_NAME} PRIVATE ${MODULE_ABSOLUTE_PATH}/${MODULE_HEADERS_DIR})
target_include_directories(${SV_NAME} PRIVATE .)


target_include_directories(${SV_NAME} PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${SV_NAME}>
    $<INSTALL_INTERFACE:include/${SV_NAME}>
)

target_include_directories(${SV_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

add_subdirectory(${CHOSEN_SIMULATOR_VIEWER})

target_link_libraries(${SV_NAME} ${CHOSEN_SIMULATOR_VIEWER})
