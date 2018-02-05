cmake_minimum_required(VERSION 2.4.7)
PROJECT(appserver4phone)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/../../../cmake_modules/")

find_package(USB-1 REQUIRED)
find_package(OpenCV REQUIRED)

include_directories($(USB_1_INCLUDE_DIR))
include_directories($(OpenCV_INCLUDE_DIR))


set(APP_SRC
intercam.cpp
log4z.cpp
resetcam.cpp
)

set(CAMIP_SRC
camip.cpp
log4z.cpp
)

set(DATETIME_SRC
correctdatetime.cpp
)

set(MPSSE_SRC
mpsse.c
)

add_executable(appserver4phone ${APP_SRC})
target_link_libraries(appserver4phone -lqhy -lmysqlclient -luuid -lz -lcfitsio ${OpenCV_LIBRARIES} ${LIBUSB_1_LIBRARIES})

add_executable(appserverip ${CAMIP_SRC})
target_link_libraries(appserverip -lpthread -lmysqlclient)

add_executable(correctdatetime ${DATETIME_SRC})

add_executable(mpsse ${MPSSE_SRC})
target_link_libraries(mpsse -lmpsse)

