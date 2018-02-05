# Error during code Single Frame Mode

- [fatal error: libusb.h: No such file or directory](https://github.com/OpenKinect/libfreenect2/issues/196)

``` C++
// File: include/qhycam.h
#include <libusb-1.0/libusb.h>
//#include <libusb.h> not found during complied
```
- [fatal error: cv.h: No such file or directory](http://blog.csdn.net/emperorjade/article/details/49718985)
``` C++
// File: SingleFrameMode/SingleFrameMode.cpp
//#include <cv.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
```

- [OpenCV Link Error](https://docs.opencv.org/trunk/db/df5/tutorial_linux_gcc_cmake.html
)
````bash
[igem@qblab-214 build]$ make
[ 50%] Linking CXX executable ./LiveFrameSampletest
CMakeFiles/./LiveFrameSampletest.dir/LiveFrameSample.o: In function `main':
LiveFrameSample.cpp:(.text+0x47e): undefined reference to `cvNamedWindow'
LiveFrameSample.cpp:(.text+0x51f): undefined reference to `cvCreateImageHeader'
LiveFrameSample.cpp:(.text+0x542): undefined reference to `cvShowImage'
LiveFrameSample.cpp:(.text+0x54c): undefined reference to `cvWaitKey'
collect2: error: ld returned 1 exit status
make[2]: *** [CMakeFiles/./LiveFrameSampletest.dir/build.make:95: LiveFrameSampletest] Error 1
make[1]: *** [CMakeFiles/Makefile2:68: CMakeFiles/./LiveFrameSampletest.dir/all] Error 2
make: *** [Makefile:84: all] Error 2
```
Add following to CMakeLists.txt
``` bash
find_package( OpenCV  REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
target_link_libraries( SingleFrameMode ${OpenCV_LIBS} )
```
