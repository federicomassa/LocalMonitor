set (CMAKE_CXX_STANDARD 11)

#Setup CMake to run tests
enable_testing()

#Prep ourselves for compiling boost
find_package(Boost COMPONENTS unit_test_framework REQUIRED)
include_directories (${Boost_INCLUDE_DIRS})

#Qt5 is REQUIRED
find_package(Qt5Widgets QUIET)
if (NOT Qt5Widgets_FOUND)
  message(FATAL_ERROR "Qt5Widgets was not found. \n \
Try calling cmake -DCMAKE_PREFIX_PATH=<path/to/Qt5WidgetsConfig.cmake, \n \
usually under <Qt root/version/gcc_64/lib/cmake/Qt5Widgets \n \
Deleting CMakeCache could be necessary")
endif(NOT Qt5Widgets_FOUND)


set(MODULE_HEADERS_DIR
    ./
)

set(MODULE_SOURCE_DIR
    ./
)

set(MODULE_EXECUTABLES
	TBasicPlot
	)
	
foreach(EXEC IN ITEMS ${MODULE_EXECUTABLES})
	add_executable(${EXEC}
	${EXEC}.cpp)
	target_link_libraries(${EXEC} Basic)
	target_link_libraries(${EXEC} Utility)
	target_link_libraries(${EXEC} ${Boost_LIBRARIES})
	target_link_libraries(${EXEC} Qt5::Widgets)
	target_link_libraries(${EXEC} QCustomPlot)
endforeach(EXEC)
