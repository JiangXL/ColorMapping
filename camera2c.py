#!/usr/bin/python
# -*- coding: utf-8 -*-

"""
Initation and configuration of openCV
"""
import cv2
import time
import numpy as np

storepath = time.strftime("%m%d", time.localtime())+"/"+time.strftime("%H",time.localtime())

## select VideoCapture device
cap = cv2.VideoCapture(0)
cap.set(3,800)
cap.set(4,600)
def live():
    ret, frame = cap.read()
    frame=cv2.transpose(frame)
    return frame

def seq_capt(interval):
  ## TIFF :
  try:
    record = open(storepath+"log.txt", 'w')
    record.write("Start Time:"+time.strftime("%Y-%m-%d-%H:%M",time.localtime()))
    record.write("\n"+str(cam_sav.getParameters()))
    #record.write(data)
  finally:
    if record:
      record.close()

live()
