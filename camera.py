#!/usr/bin/python
# -*- coding: utf-8 -*-

"""
Initation and configuration of Andor Zaly 5
"""

import time
from Lima import Core,Andor3

storepath = "/mnt/DataHub3/iGEM/"+time.strftime("%m%d", time.localtime())+"/"

cam = Andor3.Camera("/usr/local/bf", 0) # I don't kown what about Zyla USB3.0 W
cam_int = Andor3.Interface(cam)
cam_ctr = Core.CtControl(cam_int)


## saving pathway
cam_sav = cam_ctr.saving()
cam_sav.setDirectory(storepath)
#cam_sav.setPrefix("test")
cam_sav.setPrefix(time.strftime("%h%d%H", time.localtime()))
cam_sav.setSavingMode(Core.CtSaving.AutoFrame)
# AutoFrame: Save a frame just after it acquisition


## TIFF :
cam_sav.setFormat(Core.CtSaving.TIFFFormat)
cam_sav.setSuffix(".tiff")

cam_sav.getParameters()


## camera setting

cam_ctr.image().setMode(Core.CtImage.HardOnly)
cam_ctr.image().setRoi(Core.Roi(0, 0, 2048, 2048)) ### left, top, width, height

cam_ctr.acquisition().setAcqExpoTime(.010)
cam_ctr.acquisition().setLatencyTime(.10) # I don't kown the limit of hardware
cam_ctr.acquisition().setAcqNbFrames(10)



print("Capture")
cam.setNbFrames(1)
cam_ctr.prepareAcq()
cam_ctr.startAcq()

if ( Core.AcqReady == cam_ctr.getStatus().AcquisitionStatus ):
  data=cam_ctr.ReadImage().buffer
  print("test")
## record experiment to file
try:
  record = open(storepath+"log.txt", 'w')
  record.write("Start Time:"+time.strftime("%Y-%m-%d-%H:%M",time.localtime())+"\n")
  record.write(str(cam_sav.getParameters()))
  #record.write(data)
finally:
  if record:
    record.close()
