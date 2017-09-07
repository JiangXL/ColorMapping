import sys,time,os
from Lima import Core,Andor3


cam = Andor3.Camera("/usr/local/andor/bitflow", 0)
cam_int = Andor3.Interface(cam)
cam_ctr = Core.CtControl(cam_int)

## Proper way of doing that is :
cam_ctr.acquisition().setAcqExpoTime(.001)
cam_ctr.acquisition().setLatencyTime(.001)
cam_ctr.image().setMode(Core.CtImage.HardOnly)
##cam_ctr.image().setMode(Core.CtImage.HardAndSoft)
cam_ctr.image().setRoi(Core.Roi(0, 0, 2048, 2048))  ### left, top, width, height


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
fig.canvas.manager.window.after(1, animate)
plt.show()

if __name__ == "__main__":
    animate()
