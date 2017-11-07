cmake_minimum_required(VERSION 3.0.0)

set(QTFF_NAME
    QTFFmpegWrapper)

set(QTFF_HEADERS_DIR
    .
)

set(QTFF_SOURCE_DIR
    .
)

set(QTFF_SOURCES
	QVideoEncoder.cpp
	)

      
#Find<package>.cmake files are Under directory 'cmake' 
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
      
find_package(FFmpeg REQUIRED)

#Qt5 is REQUIRED
find_package(Qt5Widgets QUIET)
if (NOT Qt5Widgets_FOUND)
  message(FATAL_ERROR "Qt5Widgets was not found. \n \
Try calling cmake -DCMAKE_PREFIX_PATH=<path/to/Qt5WidgetsConfig.cmake, \n \
usually under <Qt root/version/gcc_64/lib/cmake/Qt5Widgets \n \
Deleting CMakeCache could be necessary")
endif(NOT Qt5Widgets_FOUND)

foreach(SOURCE IN ITEMS ${QTFF_SOURCES})
	set(QTFF_SOURCE ${QTFF_SOURCE} ${QTFF_SOURCE_DIR}/${SOURCE})
endforeach(SOURCE)

project(${QTFF_NAME})

add_library(${QTFF_NAME} STATIC
  ${QTFF_SOURCE}
  )

if (FFMPEG_FOUND)
  message("Found FFMPEG/LibAV libraries")
  include_directories(${FFMPEG_INCLUDE_DIR})
  target_link_libraries (${QTFF_NAME} ${FFMPEG_LIBRARIES})
else (FFMPEG_FOUND)
  message("Can't find libavcodec, libavformat or libavutil. Add them!")
endif (FFMPEG_FOUND)

target_link_libraries (${QTFF_NAME} "-lm -lswresample -lm -lz -lpthread -ldl -llzma -lva -lX11 -lva-x11 -lvdpau -lva-drm")
target_link_libraries(${QTFF_NAME} Qt5::Widgets)


target_include_directories(${QTFF_NAME} PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${QTFF_HEADERS_DIR}>
)

#From outside you can include like this: #include "Utility/QTFFmpegWrapper/QVideoEncoder.h"
target_include_directories(${QTFF_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../..>
    $<INSTALL_INTERFACE:>
)
