cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(SV_NAME
    SimulatorViewer)

set(SV_HEADERS_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/include/${SV_NAME}
)

set(SV_SOURCE_DIR
    src
)

set(SV_SOURCES
	SimulatorViewer.cpp
	)

set(SV_INCLUDES
	SimulatorViewer.h)


foreach(SV_HEADER IN ITEMS ${SV_INCLUDES})
	set(SV_INCLUDE ${SV_INCLUDE} ${SV_HEADERS_DIR}/${SV_HEADER})
endforeach(SV_HEADER)
	
	
foreach(SOURCE IN ITEMS ${SV_SOURCES})
	set(SV_SOURCE ${SV_SOURCE} ${SV_SOURCE_DIR}/${SOURCE})
endforeach(SOURCE)

project(${SV_NAME})

#qt5_wrap_cpp(SV_MOC_OUT ${SV_HEADER})

#message (STATUS "SV_HEADER " ${SV_HEADER})

add_library(${SV_NAME} STATIC
  ${SV_SOURCE}
  ${SV_INCLUDE}
  )
  
find_package(Qt5Widgets REQUIRED)
  
target_link_libraries(${SV_NAME} Utility)

# TODO: this could be unnecessary (only use necessary sources?)
target_link_libraries(${SV_NAME} Basic)
target_link_libraries(${SV_NAME} Automation)
target_link_libraries(${SV_NAME} SimulatorLibrary)
target_link_libraries(${SV_NAME} Qt5::Widgets)
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
