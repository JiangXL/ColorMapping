
/sbin/modprobe v4l2_common
/sbin/modprobe videodev
/sbin/insmod /usr/local/mod/bitflow.ko fwDelay1=200 customFlags=1
chmod a+rw /dev/video*
