set (CMAKE_CXX_STANDARD 11)

set(MODULE_NAME
    Simulator)

project(${MODULE_NAME})
  
set(MODULE_HEADERS_DIR
    include/${MODULE_NAME}
)

set(MODULE_SOURCE_DIR
    src
)

set(MODULE_SOURCES
	Simulator.cpp
	SimulatorConfiguration.cpp
	Environment.cpp
	SimulAgent.cpp
	SimulatorViewer.cpp
	)
	
foreach(SOURCE IN ITEMS ${MODULE_SOURCES})
	set(MODULE_SOURCE ${MODULE_SOURCE} ${MODULE_SOURCE_DIR}/${SOURCE})
endforeach(SOURCE)

set(SIMULATOR_BUILD_FILE SimulatorBuildParams.h)
set(SIMULATOR_CONFIG_FILE ${MODULE_NAME}/Simulator.json)
set(INPUT_DIR ${PROJECT_INPUT_DIR})
set(INPUT_DYNAMICS_DIR ${INPUT_DIR}/Dynamics)
set(KINEMATICS_FUNC_LIST ${INPUT_DYNAMICS_DIR}/Kinematics.h)
set(SIMULATOR_VIEWER_LIST ${INPUT_VIEWER_DIR}/SimulatorViewers.h)

add_executable(${MODULE_NAME}
  ${MODULE_SOURCE}
  )
  
configure_file(${MODULE_SOURCE_DIR}/${SIMULATOR_BUILD_FILE}.in ${PROJECT_SOURCE_DIR}/${MODULE_NAME}/${MODULE_HEADERS_DIR}/${SIMULATOR_BUILD_FILE})
  
target_link_libraries(${MODULE_NAME} Observer)
target_link_libraries(${MODULE_NAME} Utility)
target_link_libraries(${MODULE_NAME} Basic)
target_link_libraries(${MODULE_NAME} Automation)
target_link_libraries(${MODULE_NAME} nlohmann_json)
target_link_libraries(${MODULE_NAME} Input)



target_include_directories(${MODULE_NAME} PRIVATE ${nlohmann_json_INCLUDE_DIRS})

target_include_directories(${MODULE_NAME} PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${MODULE_NAME}>
    $<INSTALL_INTERFACE:include/${MODULE_NAME}>
)

target_include_directories(${MODULE_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

#############################
set (CONFIGURE_MODULE_NAME configure)
set (CONFIGURE_SOURCE_DIR src)
set (CONFIGURE_MODULE_SOURCES 
	configure.cpp
	)

foreach(SOURCE IN ITEMS ${CONFIGURE_MODULE_SOURCES})
	set (CONFIGURE_MODULE_SOURCE ${CONFIGURE_MODULE_SOURCE} ${CONFIGURE_SOURCE_DIR}/${SOURCE})
endforeach(SOURCE)
	
add_executable(${CONFIGURE_MODULE_NAME} ${CONFIGURE_MODULE_SOURCE})

target_link_libraries(${CONFIGURE_MODULE_NAME} Utility)

