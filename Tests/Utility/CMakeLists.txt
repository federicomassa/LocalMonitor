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
  TQTFFmpegWrapper
	)

set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTORCC ON)

find_package(Qt5Widgets REQUIRED)

foreach(EXEC IN ITEMS ${MODULE_EXECUTABLES})
	add_executable(${EXEC}
	${EXEC}.cpp resources.qrc mywidget.cpp)
	target_link_libraries(${EXEC} ${Boost_LIBRARIES})
	target_link_libraries(${EXEC} QTFFmpegWrapper)
	target_link_libraries(${EXEC} Qt5::Widgets)
endforeach(EXEC)

      
