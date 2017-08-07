# Introduction
  Under pygame and opencv, to track *C.elegans* and light single  *C.elegans* in whole view field by LCD projector.

  Andor SDK3+ Bitflow

# Now
1. OpenCV is supported, but how to do it with bitflow sdk?
2. What kind of our Camera Link?
3.

# Usage
## Prerequisite
1. lima
  ``` bash
  sudo pacman -S python-sip
  sudo pip install numpy
  ```

2. Install Andor SDK3 in  ArchLinux
``` bash
sudo pacman -S linux-headers
sudo pacman -S numactl
```
Enter the andor directory and run
``` bash
sudo ./install_andor
```
I trouble in Arch least Linux Kernel 4.11, so I install the 3.16 kernel form AUR by yaourt.
Then I add the follow configure to /etc/systemd/system

``` bash
#andorcameralink.service                                            
[Unit]
Description= Andor Camera Link

[Service]
ExecStart=/home/igem/Downloads/andor-sdk3-3.13.30001.0/andor/andorsetting.sh

[Install]
WantedBy=multi-user.target
```
The andorsetting.sh is
``` bash
#!/bin/sh
sudo modprobe v4l2_common
#sudo modprobe v4l1_compat
sudo modprobe videodev
sudo insmod /home/igem/Downloads/andor-sdk3-3.13.30001.0/andor/bitflow/drv/bitf$
sudo chmod a+rw /dev/video*
```
The andorcameralink.service is enable by
``` bash
systemctl enable andorcameralink.service
```


3.

``` bash
 sudo pip install pygame
 sudo pip install pyqt5
```



## Install

# Reading More
+ [Bitflow SDK Introduction]( www.bitflow.com/products/details/third-party-software)
+ [Bitflow Downloads](www.bitflow.com/products/downloads )
+ [NikonTi hardware triggering](https://github.com/vanNimwegenLab/MiM_NikonTi/blob/master/Docs/NikonTi_hardware_triggering.md)
+ [Lima](http://lima.blissgarden.org/camera/andor3/doc/index.html?highlight=andor3)
+ [python andor](https://pypi.python.org/pypi/andor)
+ [PyQt](https://wiki.python.org/moin/PyQt)

# License
