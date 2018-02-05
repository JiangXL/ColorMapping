# cmake_minimum_required(VERSION <specify CMake version here>)
project(QHYCCD)

set(CMAKE_CXX_STANDARD 11)

add_executable(QHYCCD
        cmake_modules/FindOpenCV.cmake
        cmake_modules/FindUSB-1.cmake
        cmake_modules/FindUSB-ARM.cmake
        common/app4phone/camip.cpp
        common/app4phone/correctdatetime.cpp
        common/app4phone/intercam.cpp
        common/app4phone/log4z.cpp
        common/app4phone/log4z.h
        common/app4phone/mpsse.c
        common/app4phone/resetcam.cpp
        common/app4phone/resetcam.h
        common/app4phone/scancam.cpp
        common/Capture.cpp
        common/ControlCFW.cpp
        common/ControlCool.cpp
        common/ForceStop.cpp
        common/GetCameraStatus.cpp
        common/GetHistJPG.cpp
        common/GetLastImageFIT.cpp
        common/GetLastImageJPG.cpp
        common/GetTemperature.cpp
        common/LiveFrameSample.cpp
        common/OSXDownloadFirmware2Camera.cpp
        common/SetTemperature.cpp
        common/SingleFrameSample.cpp
        include/qhycam.h
        include/qhyccd.h
        include/qhyccdcamdef.h
        include/qhyccderr.h
        include/qhyccdstruct.h
        LiveFrameMode/LiveFrameSample.cpp
        SingleFrameMode/SingleFrameMode.cpp
        README.md)
