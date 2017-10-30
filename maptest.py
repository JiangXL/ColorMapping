# -*- coding: utf-8 -*-
"""
Demonstrates very basic use of ImageItem to display image data
inside a ViewBox.
"""

import sys,getopt # trans the parameter
from pyqtgraph.Qt import QtCore, QtGui
import numpy as np
import pyqtgraph as pg
import pyqtgraph.ptime as ptime
import map

class maptest(QtGui.QDialog):
    def __init__(self):
        QtGui.QDialog.__init__(self)
        pg.setConfigOption('background','b')
        #pg.setConfigOption('useOpenGL',True)
        ## Create window with GraphicsView widget
        win = pg.GraphicsLayoutWidget()
        view = win.addViewBox(enableMouse=False, lockAspect=True)
        view.disableAutoRange()
        win.setCentralWidget(view) # close the edge bettwen img and window
        win.setGeometry(10,10,1024,768)
        win.setWindowTitle('Live-iGEM 2017')
        win.show()# show widget alone in its own window

        ## Create image item
        img = pg.ImageItem(border='w')
        img.setPxMode(True)
        view.addItem(img)
        view.setLimits(xMin=0,xMax=1024,yMin=-768,yMax=0)
    # Set the initial point is (0,0)

    # Video Player
    #i = 0
    #updateTime = ptime.time()
    #fps = 0

    def updateData():
        #global img, data, i, updateTime, fps
        ## Display the data
        img.setImage(map.getImage())
        #QtCore.QTimer.singleShot(1, )

        #updateData()


## Start Qt event loop unless running in interactive mode.
#if __name__ == '__main__':
#    import sys
#    if (sys.flags.interactive != 1) or not hasattr(QtCore, 'PYQT_VERSION'):
#        QtGui.QApplication.instance().exec_()
