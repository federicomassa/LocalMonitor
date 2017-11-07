cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(HV_NAME
    HighwayViewer)

set(HV_HEADERS_DIR
    include/${_NAME}
)

set(HV_SOURCE_DIR
    src
)

set(HV_SOURCES
	HighwayViewer.cpp
	)
	
foreach(SOURCE IN ITEMS ${HV_SOURCES})
	set(HV_SOURCE ${HV_SOURCE} ${HV_SOURCE_DIR}/${SOURCE})
endforeach(SOURCE)

project(${HV_NAME})

add_library(${HV_NAME} STATIC
  ${HV_SOURCE} 
  ${PROJECT_INPUT_DIR}/Simulator/${HV_NAME}/resources.qrc
  )
  
find_package(Qt5Widgets REQUIRED)
target_link_libraries(${HV_NAME} Qt5::Widgets)

target_link_libraries(${HV_NAME} SimulatorViewer)
target_link_libraries(${HV_NAME} QTFFmpegWrapper)

target_include_directories(${HV_NAME} PRIVATE ${MODULE_ABSOLUTE_PATH}/${MODULE_HEADERS_DIR})
target_include_directories(${HV_NAME} PRIVATE ${nlohmann_json_INCLUDE_DIRS})


target_include_directories(${HV_NAME} PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${HV_NAME}>
    $<INSTALL_INTERFACE:include/${HV_NAME}>
)

target_include_directories(${HV_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

