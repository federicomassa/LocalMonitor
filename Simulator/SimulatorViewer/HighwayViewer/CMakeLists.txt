cmake_minimum_required(VERSION 3.0.0)

set(HV_NAME
    HighwayViewer)


project(${HV_NAME})


set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(HV_HEADERS_DIR
    include/${HV_NAME}
)

set(HV_SOURCE_DIR
    src
)

set(HV_SOURCES
	HighwayViewer.cpp
	)
	
set(HV_INCLUDES
	HighwayViewer.h
	ui_HighwayViewer.h)
	

foreach(HV_HEADER IN ITEMS ${HV_INCLUDES})
	set(HV_INCLUDE ${HV_INCLUDE} ${HV_HEADERS_DIR}/${HV_HEADER})
endforeach(HV_HEADER)
	
	
foreach(SOURCE IN ITEMS ${HV_SOURCES})
	set(HV_SOURCE ${HV_SOURCE} ${HV_SOURCE_DIR}/${SOURCE})
endforeach(SOURCE)

#qt5_wrap_cpp(HV_MOC_OUT ${HV_HEADER})


project(${HV_NAME})

add_library(${HV_NAME} STATIC
  ${HV_SOURCE} 
  ${PROJECT_INPUT_DIR}/Simulator/${HV_NAME}/resources.qrc
  ${PROJECT_INPUT_DIR}/Simulator/HighwayViewer/ConvertState.cpp
  ${HV_INCLUDE}
  )
  
find_package(Qt5Core REQUIRED)
target_link_libraries(${HV_NAME} Qt5::Widgets)
target_link_libraries(${HV_NAME} SimulatorViewer)
#target_link_libraries(${HV_NAME} QTFFmpegWrapper)
target_link_libraries(${HV_NAME} openshot)
target_include_directories(${HV_NAME} PUBLIC ${LOCAL_MONITOR_BASE_DIR}/Utility/libopenshot-0.1.8/include)	

target_include_directories(${HV_NAME} PRIVATE ${MODULE_ABSOLUTE_PATH}/${MODULE_HEADERS_DIR})
target_include_directories(${HV_NAME} PRIVATE ${PROJECT_INPUT_DIR}/Simulator/${HV_NAME})
target_include_directories(${HV_NAME} PRIVATE ${nlohmann_json_INCLUDE_DIRS})


target_include_directories(${HV_NAME} PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${HV_NAME}>
    $<INSTALL_INTERFACE:include/${HV_NAME}>
)

target_include_directories(${HV_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

