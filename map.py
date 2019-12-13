#!/usr/bin/python
# -*- coding: utf-8 -*-

"""
Image matrix with ROI for projecting
"""
import numpy as np

# resetting image to dark
np.save("image.npy", np.zeros((1280,720,3),dtype=np.uint8).reshape(1280, 720,3))

# Input background, roi postions and color and Transcale and Set
def setRoi(bg, input):
     # Reset image to black
    image_pre = np.zeros((1280,720,3),dtype=np.uint8)
    #np.save("preimage.npy", image_pre.reshape(1024, 720,3))

    scale = 720.0/bg[3]
    for i in [0,1,2,3]:
        x0 = int((input[i,0]-bg[0])*scale)
        y0 = int((input[i,1]-bg[1])*scale)
        x1 = int(x0+(input[i,2])*scale)
        y1 = int(y0+(input[i,3])*scale)
        if x0>0 and x1<1280 and y0>0 and y1<1280:
            # set image postion and color for projecter
            image_pre[x0:x1, y0:y1,0]= int(input[i,4])
            image_pre[x0:x1, y0:y1,2]= int(input[i,5])
            print("Adding ROI", x0, y0, x1-x0, y1-y0, int(input[i,4]),input[i,5])
        else:
            print( "Out of range:")
            print( x0,y0, x1-x0, y1-y0)
    image_pre=np.flipud(image_pre)
    np.save("preimage.npy", image_pre.reshape(1280, 720,3))

def updateImage():
    np.save("image.npy", np.load("preimage.npy").reshape(1280,720,3))

# generate new image matrix
def getImage():
    while 1:
        try:
            getimage = np.load("image.npy")
            return getimage
            break
        except :
            print( "IO Busy")
