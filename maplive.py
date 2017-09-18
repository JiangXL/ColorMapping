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


app = QtGui.QApplication([])
pg.setConfigOption('background','b')
#pg.setConfigOption('useOpenGL',True)


        #win.showFullScreen()

## Create window with GraphicsView widget
win = pg.GraphicsLayoutWidget()
view = win.addViewBox(enableMouse=False, lockAspect=True)
view.disableAutoRange()
win.setCentralWidget(view) # close the edge bettwen img and window
win.setGeometry(10,10,1920,1200)
win.setWindowTitle('Live-iGEM 2017')


## Fullscreen or windos
if sys.argv[1] == "-pre":
    win.show()# show widget alone in its own window
elif sys.argv[1] in ("-full"):
    win.showFullScreen()

## Create image item
img = pg.ImageItem(border='w')
img.setPxMode(True)
view.addItem(img)
view.setLimits(xMin=0,xMax=1920,yMin=-1200,yMax=0)
# Set the initial point is (0,0)


i = 0
updateTime = ptime.time()
fps = 0

def updateData():
    global img, data, i, updateTime, fps

    ## Display the data
    img.setImage(map.getImage())

    QtCore.QTimer.singleShot(1, updateData)
    now = ptime.time()
    fps2 = 1.0 / (now-updateTime)
    updateTime = now
    fps = fps * 0.9 + fps2 * 0.1

    print ("%0.1f fps" % fps)

updateData()



## Start Qt event loop unless running in interactive mode.
if __name__ == '__main__':
    import sys
    if (sys.flags.interactive != 1) or not hasattr(QtCore, 'PYQT_VERSION'):
        QtGui.QApplication.instance().exec_()
