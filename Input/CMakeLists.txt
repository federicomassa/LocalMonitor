cmake_minimum_required(VERSION 3.0.0)

set(MODULE_NAME
    Input)

set(MODULE_HEADERS_DIR
    include/${MODULE_NAME}
)

set(MODULE_SOURCE_DIR
    ./
)

set(MODULE_SOURCES
	)
	
foreach(SOURCE IN ITEMS ${MODULE_SOURCES})
	set(MODULE_SOURCE ${MODULE_SOURCE} ${MODULE_SOURCE_DIR}/${SOURCE})
endforeach(SOURCE)

project(${MODULE_NAME})

add_library(${MODULE_NAME} STATIC
  ${MODULE_SOURCE}
  )

target_include_directories(${MODULE_NAME} PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Dynamics>
    $<INSTALL_INTERFACE:${MODULE_NAME}>
)

target_include_directories(${MODULE_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:>
)

target_link_libraries(${MODULE_NAME} Basic)
