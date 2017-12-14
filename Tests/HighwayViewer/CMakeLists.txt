set (CMAKE_CXX_STANDARD 11)

#Setup CMake to run tests
enable_testing()

project(HighwayViewerTests)

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
  TOpenShot
	)
	
foreach(EXEC IN ITEMS ${MODULE_EXECUTABLES})
	add_executable(${EXEC}
	${EXEC}.cpp)
	target_link_libraries(${EXEC} ${Boost_LIBRARIES})
endforeach(EXEC)

target_link_libraries(TOpenCVBase ${OpenCV_LIBS})
target_link_libraries(TOpenShot openshot)

target_include_directories(TOpenShot PUBLIC ${LOCAL_MONITOR_BASE_DIR}/Utility/libopenshot-0.1.8/include)	


# find_path(OPENSHOT_INCLUDE_DIR libopenshot/OpenShot.h
# 	PATHS /usr/local/include/
# 	/usr/include/)
# 	
# if (NOT OPENSHOT_INCLUDE_DIR)
# set(OPENSHOT_INCLUDE_DIR OPENSHOT_NOTFOUND)
# endif()
# 
# find_library(OPENSHOT_LIBRARY openshot
# 	PATHS
# 	/usr/lib/
# 	/usr/local/lib
# 	)
# 
# if (NOT OPENSHOT_LIBRARY)
# set(OPENSHOT_LIBRARY OPENSHOT_NOTFOUND)
# endif()
# 
# 
# if (NOT ${OPENSHOT_NOTFOUND})
# set (OPENSHOT_FOUND)
# endif()
# 
# if (NOT ${OPENSHOT_FOUND})
# MESSAGE("OPENSHOT NOT FOUND!")
# else()
# MESSAGE("Found Openshot: " ${OPENSHOT_INCLUDE_DIR} " --- " ${OPENSHOT_LIBRARY})
# endif()
# 
# target_link_libraries(TOpenShot ${OPENSHOT_LIBRARY})
# target_include_directories(TOpenShot PUBLIC ${OPENSHOT_INCLUDE_DIR})


#target_include_directories(TOpenShot PUBLIC ${LOCAL_MONITOR_BASE_DIR}/Utility/libopenshot-0.1.8/include)


# list (APPEND CMAKE_MODULE_PATH ${LOCAL_MONITOR_BASE_DIR}/Utility/libopenshot-0.1.8/cmake/Modules)
# 
# FIND_PACKAGE(ImageMagick COMPONENTS Magick++ MagickWand MagickCore)
# FIND_PACKAGE(FFmpeg REQUIRED)
# FIND_PACKAGE(OpenShotAudio REQUIRED)
# find_package(Qt5Widgets REQUIRED)
# find_package(Qt5Core REQUIRED)
# find_package(Qt5Gui REQUIRED)
# find_package(Qt5Multimedia REQUIRED)
# find_package(Qt5MultimediaWidgets REQUIRED)
# #FIND_PACKAGE(BlackMagic)
# FIND_PACKAGE(OpenMP)
# FIND_PACKAGE(ZMQ REQUIRED)
# #find_package(JsonCpp REQUIRED)
# 
# 
# target_include_directories(TOpenShot PUBLIC ${LOCAL_MONITOR_BASE_DIR}/Utility/libopenshot-0.1.8/include)
# target_include_directories(TOpenShot PUBLIC ${ImageMagick_INCLUDE_DIRS})
# target_include_directories(TOpenShot PUBLIC ${LIBOPENSHOT_AUDIO_INCLUDE_DIRS})
# target_include_directories(TOpenShot PUBLIC ${FFMPEG_INCLUDE_DIR})
# target_include_directories(TOpenShot PUBLIC ${Qt5Widgets_INCLUDE_DIRS})
# target_include_directories(TOpenShot PUBLIC ${Qt5Core_INCLUDE_DIRS})
# target_include_directories(TOpenShot PUBLIC ${Qt5Gui_INCLUDE_DIRS})
# target_include_directories(TOpenShot PUBLIC ${Qt5Multimedia_INCLUDE_DIRS})
# target_include_directories(TOpenShot PUBLIC ${Qt5MultimediaWidgets_INCLUDE_DIRS})
# #target_include_directories(TOpenShot PUBLIC ${BLACK_MAGIC_INCLUDE_DIR})
# target_include_directories(TOpenShot PUBLIC ${ZMQ_INCLUDE_DIRS})
# #target_include_directories(TOpenShot PUBLIC ${JSONCPP_INCLUDE_DIRS})
