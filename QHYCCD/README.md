## Design



## Error during code Single Frame Mode


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

-
``` bash
[igem@qblab Worm]$ make
[ 50%] Linking CXX executable ./LiveFrameSampletest
/usr/bin/ld: warning: libgtkglext-x11-1.0.so.0, needed by /usr/lib/libopencv_highgui.so.3.4.1, not found (try using -rpath or -rpath-link)
/usr/bin/ld: warning: libgdkglext-x11-1.0.so.0, needed by /usr/lib/libopencv_highgui.so.3.4.1, not found (try using -rpath or -rpath-link)
/usr/lib/libopencv_highgui.so.3.4.1: undefined reference to `gtk_widget_get_gl_window'
/usr/lib/libopencv_highgui.so.3.4.1: undefined reference to `gdk_gl_drawable_make_current'
/usr/lib/libopencv_highgui.so.3.4.1: undefined reference to `gtk_widget_set_gl_capability'
/usr/lib/libopencv_highgui.so.3.4.1: undefined reference to `gdk_gl_config_new_by_mode'
/usr/lib/libopencv_highgui.so.3.4.1: undefined reference to `gdk_gl_drawable_gl_end'
/usr/lib/libopencv_highgui.so.3.4.1: undefined reference to `gtk_widget_get_gl_context'
/usr/lib/libopencv_highgui.so.3.4.1: undefined reference to `gdk_gl_drawable_is_double_buffered'
/usr/lib/libopencv_highgui.so.3.4.1: undefined reference to `gdk_gl_drawable_gl_begin'
/usr/lib/libopencv_highgui.so.3.4.1: undefined reference to `gdk_gl_drawable_swap_buffers'
/usr/lib/libopencv_highgui.so.3.4.1: undefined reference to `gdk_gl_drawable_get_type'
/usr/lib/libopencv_highgui.so.3.4.1: undefined reference to `gtk_gl_init'
collect2: error: ld returned 1 exit status
make[2]: *** [CMakeFiles/./LiveFrameSampletest.dir/build.make:132: LiveFrameSampletest] Error 1
make[1]: *** [CMakeFiles/Makefile2:68: CMakeFiles/./LiveFrameSampletest.dir/all] Error 2
make: *** [Makefile:84: all] Error 2
```

A
``` bash
sudo pacman -S gtkglext
```
