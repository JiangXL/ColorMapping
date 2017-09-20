#!/usr/bin/python
# -*- coding: utf-8 -*-

"""

Image matrix with ROI for projecting

"""
import numpy as np


# 1920x1200 black image fill with projector
image = np.zeros((1024,768,3),dtype=np.uint8)
np.save("image.npy", image.reshape(1024, 768,3))
#Here will trouble if write and read image.npy at the same time)
# Need to reset image.npy

# Input background, roi 1, roi 2 postions
def updateRoi(bg, roi1, r1, b1, roi2, r2, b2):
    x1 = int(roi1[0]-bg[0])
    y1 = int(roi1[1]-bg[1])
    x2 = int(roi2[0]-bg[0])
    y2 = int(roi2[1]-bg[1])
    if x1<1024 and x1>0 and x2<1024 and x2>0:
        setRoi(x1, x1+int(roi1[2]), y1, y1+int(roi1[3]), int(r1), int(b1))
        setRoi(x2, x2+int(roi2[2]), y2, y2+int(roi2[3]), int(r2), int(b2))
    else:
        print "Out of range:"
        print x1,y1,r1,b1
        print x2,y2,r2,b2

# set image postion and color
def setRoi(x_start,x_end, y_start, y_end, r, b):
    #image_add = np.zeros((1024,768,3),dtype=np.uint8)
    image_add = getImage()
    image_add[x_start:x_end, y_start:y_end, 0]=r
    image_add[x_start:x_end, y_start:y_end, 2]=b
    print "update image"
    np.save("image.npy", image_add.reshape(1024, 768,3))

# generate new image matrix
def getImage():
    while 1:
        try:
            getimage = np.load("image.npy")
            return getimage
            break
        except :
            print "IO Busy"
