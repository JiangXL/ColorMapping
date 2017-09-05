# -*- coding: utf-8 -*-
"""
Demonstrates very basic use of ImageItem to display image data inside a ViewBox.
"""


from pyqtgraph.Qt import QtCore, QtGui
import numpy as np
import pyqtgraph as pg
import pyqtgraph.ptime as ptime
import camera

app = QtGui.QApplication([])
pg.setConfigOption('background','b')
#pg.setConfigOption('useOpenGL',True)

## Create window with GraphicsView widget
win = pg.GraphicsLayoutWidget()

view = win.addViewBox()
win.setCentralWidget(view)
win.show()  ## show widget alone in its own window
#win.showFullScreen()
win.setWindowTitle('Live-iGEM 2017')

## lock the aspect ratio so pixels are always square
view.setAspectLocked(True)

## Create image item
img = pg.ImageItem(border='w')
view.addItem(img)

## Set initial view bounds
#view.setRange(QtCore.QRectF(0, 0, 1920, 1200))

## Create random image
#data = np.random.normal(size=(15, 1920, 1200), loc=1024, scale=64).astype(np.uint16)
i = 0

updateTime = ptime.time()
fps = 0

def updateData():
    global img, data, i, updateTime, fps

    data = camera.live()
    img.setImage(data)
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
