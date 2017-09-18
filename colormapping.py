#!/usr/bin/python
# -*- coding: UTF-8 -*-

"""
Programe to light the C.elegans
"""

from pyqtgraph import GraphicsView
from pyqtgraph.Qt import QtGui, QtCore, USE_PYQT5
import numpy as np
import pyqtgraph as pg
import pyqtgraph.ptime as ptime
import homeui
import camera

#QtGui.QApplication.setGraphicsSystem('raster')
app = QtGui.QApplication([])

win = QtGui.QMainWindow()
win.setWindowTitle('Light for Life -- iGEM 2017')
ui = homeui.Ui_MainWindow()
ui.setupUi(win)
win.show()

# For exp time

#ui.maxSpin1.setOpts(value=255, step=1)
#ui.minSpin1.setOpts(value=0, step=1)

#ui.graphicsView.useOpenGL()  ## buggy, but you can try it if you need extra speed.

vb = pg.ViewBox()
ui.graphicsView.setCentralItem(vb)
vb.setAspectLocked()
img = pg.ImageItem()
vb.addItem(img)

# Camera setting and Image Store


ptr = 0
lastTime = ptime.time()
fps = None
def update():
    global ui, ptr, lastTime, fps, img

    expoTime = ui.spinBox_expo.value()/1000.000
    camera.setExpoTime(expoTime)
    print expoTime

    if  ui.radioButton_capture.isChecked():
        img.setImage(camera.seq_capt(ui.spinBox_Interval.value()/1000.00))
    elif ui.radioButton_pre.isChecked():
        img.setImage(camera.live())
    else:
        img.setImage(camera.live())

    ptr += 1
    now = ptime.time()
    dt = now - lastTime
    lastTime = now
    if fps is None:
        fps = 1.0/dt
    else:
        s = np.clip(dt*3., 0, 1)
        fps = fps * (1-s) + (1.0/dt) * s
    print('%0.2f fps' % fps)
    app.processEvents()  ## force complete redraw for every plot
timer = QtCore.QTimer()
timer.timeout.connect(update)
timer.start(0)



## Start Qt event loop unless running in interactive mode or using pyside.
if __name__ == '__main__':
    import sys
    if (sys.flags.interactive != 1) or not hasattr(QtCore, 'PYQT_VERSION'):
        QtGui.QApplication.instance().exec_()
