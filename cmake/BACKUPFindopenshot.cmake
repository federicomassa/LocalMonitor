# - Try to find openshot library 
# Once done this will define
#
# OPENSHOT_FOUND - system has ffmpeg or libav
# OPENSHOT_INCLUDE_DIR - the ffmpeg include directory
# OPENSHOT_LIBRARIES - Link these to use ffmpeg
#

if (OPENSHOT_LIBRARIES AND OPENSHOT_INCLUDE_DIR)
# in cache already
set(OPENSHOT_FOUND TRUE)
endif (OPENSHOT_LIBRARIES AND OPENSHOT_INCLUDE_DIR)
# use pkg-config to get the directories and then use these values
# in the FIND_PATH() and FIND_LIBRARY() calls

find_path(OPENSHOT_INCLUDE_DIR
NAMES libopenshot//OpenShot.h
PATHS
${_OPENSHOT_INCLUDE_DIRS}
/usr/include
/usr/local/include
/opt/local/include
/sw/include
PATH_SUFFIXES openshot libopenshot
)

find_library(OPENSHOT_LIBRARIES
NAMES openshot
PATHS ${_OPENSHOT_LIBRARY_DIRS} /usr/lib /usr/local/lib /opt/local/lib /sw/lib
)


if (OPENSHOT_INCLUDE_DIR AND OPENSHOT_LIBRARIES)
set(OPENSHOT_FOUND TRUE)
endif (OPENSHOT_INCLUDE_DIR AND OPENSHOT_LIBRARIES)

if (OPENSHOT_FOUND)
if (NOT OPENSHOT_FIND_QUIETLY)
message(STATUS "Found OPENSHOT: ${OPENSHOT_LIBRARIES}, ${OPENSHOT_INCLUDE_DIR}")
endif (NOT OPENSHOT_FIND_QUIETLY)
else (OPENSHOT_FOUND)
if (OPENSHOT_FIND_REQUIRED)
message(FATAL_ERROR "Could not find libopenshot")
endif (OPENSHOT_FIND_REQUIRED)
endif (OPENSHOT_FOUND)


