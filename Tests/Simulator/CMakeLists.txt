set (CMAKE_CXX_STANDARD 11)

#Setup CMake to run tests
enable_testing()

#Prep ourselves for compiling boost
find_package(Boost COMPONENTS unit_test_framework REQUIRED)

include_directories (${Boost_INCLUDE_DIRS})

if (USE_GRAPHICS)
add_definitions(-DUSE_GRAPHICS)
endif(USE_GRAPHICS)


set(MODULE_HEADERS_DIR
    ./
)

set(MODULE_SOURCE_DIR
    ./
)

set(MODULE_EXECUTABLES
  TViewers
	)

set (SIMULATOR_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../Simulator)
	
include (${SIMULATOR_DIR}/SET_SIMULATOR_VARIABLES.in OPTIONAL RESULT_VARIABLE CONFIGURATOR_FOUND)
	
foreach(EXEC IN ITEMS ${MODULE_EXECUTABLES})
	get_target_property(SIMULATOR_SOURCES Simulator SOURCES)
	
	foreach(SOURCE IN ITEMS ${SIMULATOR_SOURCES})
		if (NOT (${SOURCE} MATCHES ".+Simulator.cpp"))
			set(SIMULATOR_SOURCE ${SIMULATOR_SOURCE} ${SOURCE})
		endif(NOT (${SOURCE} MATCHES ".+Simulator.cpp"))
	endforeach(SOURCE IN ITEMS ${SIMULATOR_SOURCES})
	
	
	add_executable(${EXEC}
	${EXEC}.cpp ${SIMULATOR_SOURCE})
	target_link_libraries(${EXEC} ${Boost_LIBRARIES})
	target_link_libraries(${EXEC} SimulatorViewer)
	target_link_libraries(${EXEC} Input)
	target_link_libraries(${EXEC} Utility)
	target_link_libraries(${EXEC} Basic)
	target_link_libraries(${EXEC} Automation)
	target_include_directories(${EXEC} PRIVATE ${SIMULATOR_DIR}/include/Simulator)
	if (NOT(${CONFIGURATOR_FOUND} STREQUAL NOTFOUND))
		target_link_libraries(${EXEC} ${CHOSEN_SIMULATOR_VIEWER})
	endif (NOT(${CONFIGURATOR_FOUND} STREQUAL NOTFOUND))
	
	target_include_directories(${EXEC} PRIVATE ${nlohmann_json_INCLUDE_DIRS})
endforeach(EXEC)
