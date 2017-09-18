#!/usr/bin/python
# -*- coding: utf-8 -*-

"""
Initation and configuration of Andor Zaly 5
"""

import time
from Lima import Core,Andor3

storepath = "/mnt/DataHub3/iGEM/"+time.strftime("%m%d", time.localtime())+"/"+time.strftime("%H",time.localtime())+"/"

cam = Andor3.Camera("/usr/local/bf", 0) # I don't kown what about Zyla USB3.0 W
cam_int = Andor3.Interface(cam)
cam_ctr = Core.CtControl(cam_int)

## camera setting
cam_ctr.image().setMode(Core.CtImage.HardOnly)
cam_ctr.image().setRoi(Core.Roi(0, 0, 2048, 2048)) ### left, top, width, height


def setExpoTime(time):
    expoTime = time
    cam_ctr.acquisition().setAcqExpoTime(expoTime)

print ("expo time= %f s" % cam_ctr.acquisition().getAcqExpoTime())

def seq_capt(interval):
  cam_ctr.acquisition().setLatencyTime(1.2) # I don't kown limit of hardware
  #cam_ctr.acquisition().setAcqNbFrames(10)
  ## saving pathway
  cam_sav = cam_ctr.saving()
  cam_sav.setDirectory(storepath)
  cam_sav.setPrefix("test")
  #cam_sav.setPrefix(time.strftime("%h%d%H", time.localtime()))
  cam_sav.setSavingMode(Core.CtSaving.AutoFrame)
  # AutoFrame: Save a frame just after it acquisition

  ## TIFF :
  cam_sav.setFormat(Core.CtSaving.TIFFFormat)
  cam_sav.setSuffix(".tiff")

  cam_ctr.prepareAcq()
  cam_ctr.startAcq()

  the_wait=0
  while ( Core.AcqReady != cam_ctr.getStatus().AcquisitionStatus ) :
    time.sleep(interval)
    the_wait += interval
  print("Acquisition done with sleep of %fs"% (the_wait))
  while cam_ctr.getStatus().AcquisitionStatus: pass
  return cam_ctr.ReadImage().buffer

  time.sleep(1) # What it mean?
  try:
    record = open(storepath+"log.txt", 'w')
    record.write("Start Time:"+time.strftime("%Y-%m-%d-%H:%M",time.localtime()))
    record.write("\n"+str(cam_sav.getParameters()))
    #record.write(data)
  finally:
    if record:
      record.close()


def live():
  print ("expo time= %f s" % cam_ctr.acquisition().getAcqExpoTime())

  cam.setElectronicShutterMode(Andor3.Camera.Global)
  cam.setAdcRate(cam.MHz280)
  cam_ctr.acquisition().setLatencyTime(.015)
  cam.setNbFrames(1)

  cam_ctr.prepareAcq()
  cam_ctr.startAcq()
  #cam_ctr.video().startLive()
  #time.sleep(0.0001)
  #cam_ctr.video().stopLive() what is difference?
  while cam_ctr.getStatus().AcquisitionStatus: pass
  return cam_ctr.ReadImage().buffer

#live()
#seq_capt()
