cmake_minimum_required(VERSION 3.0.0)

set(MODULE_NAME
    Utility)

set(MODULE_HEADERS_DIR
    include/${MODULE_NAME}
)

set(MODULE_SOURCE_DIR
    src
)

set(MODULE_SOURCES
	LogFunctions.cpp
	Logger.cpp
	Interval.cpp
	)
	
foreach(SOURCE IN ITEMS ${MODULE_SOURCES})
	set(MODULE_SOURCE ${MODULE_SOURCE} ${MODULE_SOURCE_DIR}/${SOURCE})
endforeach(SOURCE)

project(MODULE_NAME)

add_library(${MODULE_NAME} STATIC
  ${MODULE_SOURCE}
  )

target_include_directories(${MODULE_NAME} PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${MODULE_NAME}>
    $<INSTALL_INTERFACE:include/${MODULE_NAME}>
)

target_include_directories(${MODULE_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

add_subdirectory(json-2.1.1)
