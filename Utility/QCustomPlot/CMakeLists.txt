cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_AUTOMOC ON)

set(QCustomPlot_NAME
    QCustomPlot)

set(QCustomPlot_DIR
    .
)

set(QCustomPlot_SOURCE_DIR
    .
)

set(QCustomPlot_SOURCES
	qcustomplot.cpp
	)

#Qt5 is REQUIRED
find_package(Qt5Widgets QUIET)
if (NOT Qt5Widgets_FOUND)
  message(FATAL_ERROR "Qt5Widgets was not found. \n \
Try calling cmake -DCMAKE_PREFIX_PATH=<path/to/Qt5WidgetsConfig.cmake, \n \
usually under <Qt root/version/gcc_64/lib/cmake/Qt5Widgets \n \
Deleting CMakeCache could be necessary")
endif(NOT Qt5Widgets_FOUND)

find_package(Qt5PrintSupport QUIET)
if (NOT Qt5PrintSupport_FOUND)
  message(FATAL_ERROR "Qt5PrintSupport was not found. \n \
Try calling cmake -DCMAKE_PREFIX_PATH=<path/to/Qt5PrintSupportConfig.cmake, \n \
usually under <Qt root/version/gcc_64/lib/cmake/Qt5PrintSupport \n \
Deleting CMakeCache could be necessary")
endif(NOT Qt5PrintSupport_FOUND)


foreach(SOURCE IN ITEMS ${QCustomPlot_SOURCES})
	set(QCustomPlot_SOURCE ${QCustomPlot_SOURCE} ${QCustomPlot_SOURCE_DIR}/${SOURCE})
endforeach(SOURCE)

project(${QCustomPlot_NAME})

add_library(${QCustomPlot_NAME} STATIC
  ${QCustomPlot_SOURCE}
  )

target_link_libraries(${QCustomPlot_NAME} Qt5::Widgets)
target_link_libraries(${QCustomPlot_NAME} Qt5::PrintSupport)

target_include_directories(${QCustomPlot_NAME} PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${QCustomPlot_HEADERS_DIR}>
)

#From outside you can include like this: #include "Utility/QCustomPlot/qcustomplot.h
target_include_directories(${QCustomPlot_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../..>
    $<INSTALL_INTERFACE:>
)
