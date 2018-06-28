# H.F. 20180627

cmake_minimum_required(VERSION 2.4.7)
PROJECT(Player)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake_modules/")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

find_package(USB-1 REQUIRED)
find_package(OpenCV REQUIRED)

include_directories($(USB_1_INCLUDE_DIR))
include_directories($(OpenCV_INCLUDE_DIR))

include_directories("/usr/local/include/")
link_directories("/usr/local/lib/")

add_executable(Player main.cpp camera.cpp image.cpp actionmap.cpp)

target_link_libraries(Player -lqhy ${LIBUSB_1_LIBRARIES})
target_link_libraries(Player -lqhy ${OpenCV_LIBS} )
