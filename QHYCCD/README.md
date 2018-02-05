fatal error: libusb.h: No such file or directory
https://github.com/OpenKinect/libfreenect2/issues/196

fatal error: cv.h: No such file or directory
http://blog.csdn.net/emperorjade/article/details/49718985


``bash
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
https://docs.opencv.org/trunk/db/df5/tutorial_linux_gcc_cmake.html
