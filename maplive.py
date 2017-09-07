# -*- coding: utf-8 -*-
"""
Demonstrates very basic use of ImageItem to display image data
inside a ViewBox.
"""


from pyqtgraph.Qt import QtCore, QtGui
import numpy as np
import pyqtgraph as pg
import pyqtgraph.ptime as ptime
import map


app = QtGui.QApplication([])
pg.setConfigOption('background','b')
#pg.setConfigOption('useOpenGL',True)

## Create window with GraphicsView widget
win = pg.GraphicsLayoutWidget()
view = win.addViewBox(enableMouse=False, lockAspect=True)
view.disableAutoRange()
win.setCentralWidget(view) # close the edge bettwen img and window
win.setGeometry(0,0,1920,1200)
win.show()  ## show widget alone in its own window
#win.showFullScreen()
win.setWindowTitle('Live-iGEM 2017')


## Create image item
img = pg.ImageItem(border='w')
img.setPxMode(True)

view.addItem(img)
view.setLimits(xMin=0,xMax=1920,yMin=-1200,yMax=0)
# Set the initial point is (0,0)

image = np.zeros((1920,1200,3),dtype=np.uint8)

# Add Roi to image
def setRoi(x_start,x_end, y_start, y_end, r, g, b):
    image[x_start:x_end, y_start:y_end, 0]=r
    image[x_start:x_end, y_start:y_end, 1]=g
    image[x_start:x_end, y_start:y_end, 2]=b
    #np.save("image.npy", image.reshape(1920, 1200,3))

i = 0
updateTime = ptime.time()
fps = 0

def updateData():
    global img, data, i, updateTime, fps

    ## Display the data
    img.setImage(image)

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
