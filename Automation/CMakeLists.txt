cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_CXX_STANDARD 11)

set(MODULE_NAME
    Automation)

set(MODULE_HEADERS_DIR
    include/${MODULE_NAME}
)

set(MODULE_SOURCE_DIR
    src
)

set(MODULE_SOURCES
	ControllerFailure.cpp
	PhysicalLayer.cpp
	DynamicModel.cpp
	ControlModel.cpp
	Automaton.cpp
	ExternalSensor.cpp
	InternalSensor.cpp
	SensorOutput.cpp
	Controller.cpp
	SubEvent.cpp
	Event.cpp
	Transition.cpp
	)
	
foreach(SOURCE IN ITEMS ${MODULE_SOURCES})
	set(MODULE_SOURCE ${MODULE_SOURCE} ${MODULE_SOURCE_DIR}/${SOURCE})
endforeach(SOURCE)

project(${MODULE_NAME})

add_library(${MODULE_NAME} STATIC
  ${MODULE_SOURCE}
  )
  
target_link_libraries(${MODULE_NAME} Basic)
#target_link_libraries(${MODULE_NAME} Input)

target_include_directories(${MODULE_NAME} PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${MODULE_NAME}>
    $<INSTALL_INTERFACE:include/${MODULE_NAME}>
)

target_include_directories(${MODULE_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

