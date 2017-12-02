cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_CXX_STANDARD 11)

set(MODULE_NAME
    Input)

set(MODULE_HEADERS_DIR
    include/${MODULE_NAME}
)

set(MODULE_SOURCE_DIR
    .
)

set(MODULE_SOURCES
	Automation/Controllers/AccOmegaControl.cpp
    Automation/Sensors/VisionSensor.cpp
    Dynamics/DynamicModels.cpp
    Dynamics/StateConversions.cpp
    Automation/Controllers/Controllers.cpp
    Automation/Sensors/Sensors.cpp
	)
	
foreach(SOURCE IN ITEMS ${MODULE_SOURCES})
	set(MODULE_SOURCE ${MODULE_SOURCE} ${MODULE_SOURCE_DIR}/${SOURCE})
endforeach(SOURCE)

project(${MODULE_NAME})

add_library(${MODULE_NAME} STATIC
  ${MODULE_SOURCE}
  )

target_include_directories(${MODULE_NAME} PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${MODULE_NAME}>
)

# to be called with Input/
target_include_directories(${MODULE_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
    $<INSTALL_INTERFACE:>
)

target_link_libraries(${MODULE_NAME} Basic)
