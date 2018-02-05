#===========================================================================
# Title      : CMakeLists.txt
# Note       : This cmake generates QHYCCD's SingleFrameMode test application.
# Written by : Jan Soldan
# Last Edited: H.F.
# Version    : 1.1
# Date       : 2/2018
#===========================================================================
cmake_minimum_required(VERSION 2.6)
PROJECT(SingleFrameMode)

message("PROJECT_SOURCE_DIR: " ${PROJECT_SOURCE_DIR})

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/../cmake_modules/")
set(CMAKE_OSX_ARCHITECTURES i386)

find_package(USB-1 REQUIRED)
include_directories($(USB_1_INCLUDE_DIR))

# add by H.F.
find_package( OpenCV  REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

include_directories("/usr/local/include/libusb-1.0")
link_directories("/usr/local/lib/")

add_executable(SingleFrameMode SingleFrameMode)
target_link_libraries(SingleFrameMode -lqhy)
target_link_libraries( SingleFrameMode ${OpenCV_LIBS} )
