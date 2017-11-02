set (CMAKE_CXX_STANDARD 11)

#Setup CMake to run tests
enable_testing()

#Prep ourselves for compiling boost
find_package(Boost COMPONENTS unit_test_framework REQUIRED)

find_package(OpenCV REQUIRED)

include_directories (${Boost_INCLUDE_DIRS})

set(MODULE_HEADERS_DIR
    ./
)

set(MODULE_SOURCE_DIR
    ./
)

set(MODULE_EXECUTABLES
  TOpenCVBase
	)
	
foreach(EXEC IN ITEMS ${MODULE_EXECUTABLES})
	add_executable(${EXEC}
	${EXEC}.cpp)
	target_link_libraries(${EXEC} ${Boost_LIBRARIES})
	target_link_libraries(${EXEC} ${OpenCV_LIBS})
endforeach(EXEC)

      
