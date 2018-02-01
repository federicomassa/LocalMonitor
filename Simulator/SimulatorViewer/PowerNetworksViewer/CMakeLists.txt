cmake_minimum_required(VERSION 3.0.0)

set(PNV_NAME
    PowerNetworksViewer)


project(${PNV_NAME})


set(CMAKE_CXX_STANDARD 14)

set(PNV_HEADERS_DIR
    include/${HV_NAME}
)

set(PNV_SOURCE_DIR
    src
)

set(PNV_SOURCES
	PowerNetworksViewer.cpp
	)
	
foreach(SOURCE IN ITEMS ${PNV_SOURCES})
	set(PNV_SOURCE ${PNV_SOURCE} ${PNV_SOURCE_DIR}/${SOURCE})
endforeach(SOURCE)

project(${PNV_NAME})

add_library(${PNV_NAME} STATIC
  ${PNV_SOURCE} 
  )
  
target_link_libraries(${PNV_NAME} SimulatorViewer)
target_link_libraries(${PNV_NAME} Utility)
target_include_directories(${PNV_NAME} PRIVATE ${nlohmann_json_INCLUDE_DIRS})


target_include_directories(${PNV_NAME} PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${PNV_NAME}>
    $<INSTALL_INTERFACE:include/${PNV_NAME}>
)

target_include_directories(${PNV_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

