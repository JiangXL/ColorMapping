#!/usr/bin/python
# -*- coding: utf-8 -*-

"""

Image matrix with ROI for projecting

"""
import numpy as np


# 1920x1200 black image fill with projector
image = np.zeros((1920,1200,3),dtype=np.uint8)
np.save("image.npy", image.reshape(1920, 1200,3))
#Here will trouble if write and read image.npy at the same time)
# Need to reset image.npy

# Input background, roi 1, roi 2 postions
def posInput(bg, roi1, roi2):
    print roi1
    print bg[0]



def setRoi(x_start,x_end, y_start, y_end, r, b):

    image[x_start:x_end, y_start:y_end, 0]=r
    image[x_start:x_end, y_start:y_end, 2]=b
    np.save("image.npy", image.reshape(1920, 1200,3))

# generate new image matrix
def getImage():
    getimage = np.load("image.npy")
    return getimage
