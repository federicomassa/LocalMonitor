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

# TODO Automatize insertion of user-defined classes (controllers, automatons and such)
set(MODULE_SOURCES
	Automation/Controllers/AccOmegaControl.cpp
	Automation/Controllers/StraightLineControl.cpp
	Automation/Controllers/PowNetController.cpp
	Automation/Controllers/PowNetConstantController.cpp
    Automation/Sensors/VisionSensor.cpp
    Automation/Sensors/SelfSensor.cpp
    Automation/Sensors/PowNetInternalSensor.cpp
    Automation/Sensors/PowNetExternalSensor.cpp
    Dynamics/DynamicModels.cpp
    Dynamics/PowNetDynamics.cpp
    Dynamics/StateConversions.cpp
    Automation/Controllers/Controllers.cpp
    Automation/Controllers/ControllerFailures.cpp
    Automation/Controllers/ZeroControlFailure.cpp
    Automation/Sensors/Sensors.cpp
    Automation/Automatons/Automatons.cpp
    Automation/Automatons/TestAutomaton.cpp
    Automation/Automatons/PowNetDistrAutomaton.cpp
	Automation/Automatons/PowNetConstAutomaton.cpp
    Observers/Observers.cpp
    Observers/NaiveObserver.cpp
    Observers/NaiveEnvironment.cpp
	)
	
foreach(SOURCE IN ITEMS ${MODULE_SOURCES})
	set(MODULE_SOURCE ${MODULE_SOURCE} ${MODULE_SOURCE_DIR}/${SOURCE})
endforeach(SOURCE)

project(${MODULE_NAME})

add_library(${MODULE_NAME} SHARED
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
target_link_libraries(${MODULE_NAME} Automation)
target_link_libraries(${MODULE_NAME} Observer)
target_link_libraries(${MODULE_NAME} -lstdc++fs)

