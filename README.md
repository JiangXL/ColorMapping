# Introduction

Track *C.elegans* and light single  *C.elegans* inside microscope by LCD
projector. This project is code by PyQt5 and Python2 in anaconda, which is
tested on Archlinux.

``` bash
  ~~~~~~~~~~~~	      ~~~~~~~~~~      ~~~~~~~~~~~~~~~~~~~~      ~~~~~~~~~~~~~~
  |	     |	      |	       |      |  User Interface: |      |            |
  |          |	      |        |      | 1. Show Image    |	| Project    |
  |          |    \   |	       |   \  |	2. Select ROI    |   \	| image	     |
  | Camera:  |-----\  | Andor  |----\ |	3. Set Parameters|----\ | into 	     |
  |Andor Zyla|-----/  | SDK3   |----/ |	 --------------- |----/	| Microscope |
  |   (5.2)  |	  /   |	       |   /  |	 Calculation:	 |   /	|	     |
  |          |	      |        |      | Generate image   |	|	     |
  ~~~~~~~~~~~~	      ~~~~~~~~~~      ~~~~~~~~~~~~~~~~~~~~	~~~~~~~~~~~~~~
```



# Prerequisite


1. Install Andor SDK3 in  ArchLinux

``` bash
# prepare tool for kenel compile
sudo pacman -S linux-headers
sudo pacman -S numactl
# Andor sdk3 only run below linux kernel 4, so I install linux kernel 3.16
yaourt linux-lts316
#Enter the andor directory and run
sudo ./install_andor
#Then I add the follow configure to /etc/systemd/system
#andorcameralink.service                                            

[Unit]
Description= Andor Camera Link

[Service]
ExecStart=/home/igem/Bin/andor/andorsetting.sh

[Install]
WantedBy=multi-user.target
```

The andorsetting.sh is
``` bash
#!/bin/sh
sudo modprobe v4l2_common
#sudo modprobe v4l1_compat
sudo modprobe videodev
sudo insmod /usr/local/mod/bitflow.ko fwDealy1=200 customFlags=1
sudo chmod a+rw /dev/video*
```

The andorcameralink.service is enable by
``` bash
systemctl enable andorcameralink.service
```

Add the nopat kernel option to the bootloader by editing /etc/default/grub,
adding nopat to the line GRUB_CMDLINE_LINUX_DEFAULT and running sudo
update-grub. You can check that the option is active by printing out
/proc/cmdline, if it is not not the case, reboot your computer.

The offical examples can run.

All software run but show AT_ERR_NODATA during live, I switch PCI slot
and clost C-state in DELL bios. Finally work!


2. Compilation and  installatin of Lima
  * Dependents
``` bash
sudo pacman -S python-sip
sudo pip install numpy  
```
  * Compilation
``` bash
# generate config.inc
make
# Edit the configuration file config.inc
...
COMPILE_CORE=1
COMPILE_ANDOR3=1
COMPILE_TIFF_SAVING=1
COMPILE_HDF5_SAVING=1
...
# Configure all python modules
make config
# Finally compile all C++ libraries
make
# Compile all Python modules
make -C sip -j3
```
  * Installation
```bash
sudo make install
# Update environment for python and library paths
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:<my-new-install-dir>/Lima/lib
export PYTHONPATH=$PYTHONPATH:<my-new-install-dir>
```



3. Install Micro-manager in ArchLinux
Following office document and commit from [micromanager AUR](https://aur.archlinux.org/packages/micromanager-git)


## Install


# References

+ [PyQtGraph](http://www.pyqtgraph.org/)
+ [PyQtGraph API](http://www.pyqtgraph.org/documentation/apireference.html)
+ [Lima  with andor sdk3](http://lima.blissgarden.org/camera/andor3/doc/index.html?highlight=andor3)
+ [Andor sdk3 Micro-manager](https://micro-manager.org/wiki/AndorSDK3)
+ [Andor install guide from Libuca](https://github.com/ufo-kit/uca-andor)
+ [ERR11: AT_ERR_NODATA](http://micro-manager.3463995.n2.nabble.com/ANDOR-sCMOS-configuration-issues-td7587110.html)

# License
GNU General Public License v3.0
