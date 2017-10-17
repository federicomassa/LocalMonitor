set (CMAKE_CXX_STANDARD 11)

#Setup CMake to run tests
enable_testing()

#Prep ourselves for compiling boost
find_package(Boost COMPONENTS unit_test_framework REQUIRED)
include_directories (${Boost_INCLUDE_DIRS})


set(MODULE_NAME
    TestAutomation)

set(MODULE_HEADERS_DIR
    ./
)

set(MODULE_SOURCE_DIR
    ./
)

set(MODULE_SOURCES
	TPhysicalLayer.cpp
	
	)
	
foreach(SOURCE IN ITEMS ${MODULE_SOURCES})
	set(MODULE_SOURCE ${MODULE_SOURCE} ${MODULE_SOURCE_DIR}/${SOURCE})
endforeach(SOURCE)

add_executable(${MODULE_NAME}
  ${MODULE_SOURCE}
  )
  
target_link_libraries(${MODULE_NAME} Automation)
target_link_libraries(${MODULE_NAME} Input)
target_link_libraries(${MODULE_NAME} Utility)
target_link_libraries(${MODULE_NAME} ${Boost_LIBRARIES})
