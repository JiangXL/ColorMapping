#!/usr/bin/python
# -*- coding: utf-8 -*-

"""
Image matrix with ROI for projecting

"""
import sys
import numpy as np


# 1920x1200 black image fill with projector
image = np.zeros((1920,1200,3),dtype=np.uint8)
np.save("image.npy", image.reshape(1920, 1200,3))

# Add Roi to image
def setRoi(x_start,x_end, y_start, y_end, r, g, b):
    image[x_start:x_end, y_start:y_end, 0]=r
    image[x_start:x_end, y_start:y_end, 1]=g
    image[x_start:x_end, y_start:y_end, 2]=b
    np.save("image.npy", image.reshape(1920, 1200,3))

# generate new image matrix
def getImage():
    getimage = np.load("image.npy")
    #getimage.reshape(1920,1200,3)
    return getimage
