set (CMAKE_CXX_STANDARD 14)

set(MODULE_NAME
    Simulator)

set(SIMULATOR_MODULE_NAME ${MODULE_NAME})
    
set(MODULE_HEADERS_DIR
    include/${MODULE_NAME}
)

#set (CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(MODULE_ABSOLUTE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

set(MODULE_SOURCE_DIR
    src
)

set(MODULE_SOURCES_NOEXEC
	SimulatorConfiguration.cpp
	Environment.cpp
	SimulAgent.cpp
	)

set(MODULE_SOURCES ${MODULE_SOURCES_NOEXEC} Simulator.cpp)
	
if (USE_GRAPHICS)
add_definitions(-DUSE_GRAPHICS)
endif(USE_GRAPHICS)

find_package(Qt5Widgets REQUIRED)
	
foreach(SOURCE IN ITEMS ${MODULE_SOURCES})
  set(MODULE_SOURCE ${MODULE_SOURCE} ${MODULE_SOURCE_DIR}/${SOURCE})
  set(MODULE_ABSOLUTE_SOURCE ${MODULE_ABSOLUTE_SOURCE} ${MODULE_ABSOLUTE_PATH}/${MODULE_SOURCE_DIR}/${SOURCE})
endforeach(SOURCE)

foreach(SOURCE IN ITEMS ${MODULE_SOURCES_NOEXEC})
  set(MODULE_SOURCE_NOEXEC ${MODULE_SOURCE_NOEXEC} ${MODULE_SOURCE_DIR}/${SOURCE})
  set(MODULE_ABSOLUTE_SOURCE_NOEXEC ${MODULE_ABSOLUTE_SOURCE_NOEXEC} ${MODULE_ABSOLUTE_PATH}/${MODULE_SOURCE_DIR}/${SOURCE})
endforeach(SOURCE)

set(SIMULATOR_BUILD_FILE SimulatorBuildParams.h)
set(SIMULATOR_CONFIG_FILE ${MODULE_NAME}/Simulator.json)
set(INPUT_DIR ${PROJECT_INPUT_DIR})
set(OUTPUT_DIR ${PROJECT_OUTPUT_DIR})

set(INPUT_DYNAMICS_DIR ${INPUT_DIR}/Dynamics)
set(SIMULATOR_VIEWER_LIST ${INPUT_VIEWER_DIR}/SimulatorViewers.h)
include (SET_SIMULATOR_VARIABLES.in OPTIONAL RESULT_VARIABLE CONFIGURATOR_FOUND)

add_executable(${MODULE_NAME}
  Simulator.cpp
  )


add_library(SimulatorLibrary STATIC
  ${MODULE_SOURCE_NOEXEC})



set_target_properties("${MODULE_NAME}" PROPERTIES SOURCES "${MODULE_ABSOLUTE_SOURCE}")
  
configure_file(${MODULE_SOURCE_DIR}/${SIMULATOR_BUILD_FILE}.in ${CMAKE_CURRENT_SOURCE_DIR}/${MODULE_HEADERS_DIR}/${SIMULATOR_BUILD_FILE})

target_link_libraries(${MODULE_NAME} SimulatorLibrary)
target_link_libraries(${MODULE_NAME} Automation)
target_link_libraries(${MODULE_NAME} Qt5::Widgets Qt5::Core)

target_link_libraries(SimulatorLibrary Utility)
target_link_libraries(SimulatorLibrary Basic)
target_link_libraries(SimulatorLibrary Automation)
target_link_libraries(SimulatorLibrary nlohmann_json)
target_link_libraries(SimulatorLibrary Input)
target_link_libraries(SimulatorLibrary Observer)



if (NOT(${CONFIGURATOR_FOUND} STREQUAL NOTFOUND))
target_link_libraries(${MODULE_NAME} ${CHOSEN_SIMULATOR_VIEWER})
endif (NOT(${CONFIGURATOR_FOUND} STREQUAL NOTFOUND))

target_include_directories(${MODULE_NAME} PRIVATE ${nlohmann_json_INCLUDE_DIRS})

target_include_directories(${MODULE_NAME} PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${MODULE_NAME}>
    $<INSTALL_INTERFACE:include/${MODULE_NAME}>
)

target_include_directories(${MODULE_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_include_directories(SimulatorLibrary PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${MODULE_NAME}>
    $<INSTALL_INTERFACE:include/${MODULE_NAME}>
)

target_include_directories(SimulatorLibrary PUBLIC
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

target_include_directories(${CONFIGURE_MODULE_NAME} PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${MODULE_NAME}>
    $<INSTALL_INTERFACE:include/${MODULE_NAME}>
)

target_link_libraries(${CONFIGURE_MODULE_NAME} Utility)
target_link_libraries(${CONFIGURE_MODULE_NAME} nlohmann_json)

target_include_directories(${MODULE_NAME} PRIVATE ${nlohmann_json_INCLUDE_DIRS})
target_include_directories(${MODULE_NAME} PRIVATE ${LOCAL_MONITOR_BASE_DIR}/Utility/libopenshot0.1.8/include)

add_subdirectory(SimulatorViewer)

target_link_libraries(${MODULE_NAME} SimulatorViewer)
