set (CMAKE_CXX_STANDARD 11)

#Setup CMake to run tests
enable_testing()

#Prep ourselves for compiling boost
find_package(Boost COMPONENTS unit_test_framework REQUIRED)
include_directories (${Boost_INCLUDE_DIRS})

set(MODULE_HEADERS_DIR
    ./
)

set(MODULE_SOURCE_DIR
    ./
)

set(MODULE_EXECUTABLES
	TStateRegion
	TStateCell
	)
	
foreach(EXEC IN ITEMS ${MODULE_EXECUTABLES})
	add_executable(${EXEC}
	${EXEC}.cpp)
	target_link_libraries(${EXEC} Basic)
	target_link_libraries(${EXEC} Utility)
	target_link_libraries(${EXEC} ${Boost_LIBRARIES})
endforeach(EXEC)
