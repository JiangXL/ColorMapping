#!/usr/bin/python
# -*- coding: utf-8 -*-

"""
Initation and configuration of Andor Zaly 5
"""
import sys,time,os
from Lima import Core,Andor3

cam = Andor3.Camera("/usr/local/bf", 0) # I don't kown what about Zyla USB3.0 W
cam_int = Andor3.Interface(cam)
cam_ctr = Core.CtControl(cam_int)

# cam.setNbFrames(3)

cam_sav = cam_ctr.saving()
cam_sav.setDirectory("/mnt/DataHub3/iGEM/20170903/camera")
cam_sav.setPrefix("testing")
cam_sav.setSavingMode(Core.CtSaving.AutoFrame)


## TIFF :
cam_sav.setFormat(Core.CtSaving.TIFFFormat)
cam_sav.setSuffix(".tiff")

cam_sav.getParameters()


## camera setting
cam_ctr.acquisition().setAcqExpoTime(.010)
cam_ctr.acquisition().setLatencyTime(.010)
cam_ctr.acquisition().setAcqNbFrames(10)
#cam_ctr.image().setMode(Core.CtImage.HardOnly)
cam_ctr.image().setMode(Core.CtImage.HardAndSoft)
cam_ctr.image().setRoi(Core.Roi(0, 0, 2048, 2048)) ### left, top, width, height

Core.DebParams.setTypeFlagsNameList(['Fatal','Error','Warning','Trace','Funct'])

#cam_ctr.video().startLive()
#time.sleep(3)
#cam_ctr.video().stopLive()

#cam_ctr.prepareAcq()
#cam_ctr.startAcq()
#time.sleep(1)
#cam_ctr.stopAcq()


#im1 = cam_ctr.ReadImage()
#im2 = cam_ctr.ReadImage()
#im3 = cam_ctr.ReadImage()

import time
import numpy
import numpy.fft
import matplotlib

matplotlib.use('TkAgg') # do this before importing pylab

import matplotlib.pyplot as plt

def get_image():
    cam.setNbFrames(1)
    cam_ctr.prepareAcq()
    cam_ctr.startAcq()
    while cam_ctr.getStatus().AcquisitionStatus: pass
    return cam_ctr.ReadImage().buffer

def animate():
    tstart = time.time()                   # for profiling
    data=get_image()
    im=plt.imshow(data)

    while 1:
        data=get_image()
        im.set_data(data)
        fig.canvas.draw()                         # redraw the canvas
    print 'FPS:' , 200/(time.time()-tstart)

fig = plt.figure()
ax = fig.add_subplot(111)

win = fig.canvas.manager.window
fig.canvas.manager.window.after(100, animate)
plt.show()

if __name__ == "__main__":
    animate()
