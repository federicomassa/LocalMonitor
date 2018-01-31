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
  TTimedContainer
  TIMap
	)

set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTORCC ON)

find_package(Qt5Widgets REQUIRED)

foreach(EXEC IN ITEMS ${MODULE_EXECUTABLES})
	add_executable(${EXEC} ${EXEC}.cpp)
	#${EXEC}.cpp resources.qrc mywidget.cpp)
	target_link_libraries(${EXEC} ${Boost_LIBRARIES})
endforeach(EXEC)

target_link_libraries(TTimedContainer Utility)
target_link_libraries(TIMap Utility)


      
