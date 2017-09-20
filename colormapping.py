#!/usr/bin/python
# -*- coding: UTF-8 -*-

"""
Programe to light the C.elegans
"""

from pyqtgraph.Qt import QtGui, QtCore, USE_PYQT5
import numpy as np
import pyqtgraph as pg
import pyqtgraph.ptime as ptime
import homeui
import camera
import map

#QtGui.QApplication.setGraphicsSystem('raster')
app = QtGui.QApplication([])

win = QtGui.QMainWindow()
ui = homeui.Ui_MainWindow() # set ui
ui.setupUi(win)
win.setWindowTitle('Light for Life -- iGEM 2017')
win.show()


#ui.graphicsView.useOpenGL()
vb = pg.ViewBox()
ui.graphicsView.setCentralItem(vb)
vb.setAspectLocked()
img = pg.ImageItem()
vb.addItem(img)

# Camera setting and Image Store

# ROI setting
neuron_1 = pg.RectROI([1024,1344], [100,100])
neuron_2 = pg.RectROI([1024,1000], [100,100])
bg = pg.RectROI([0,0], [200,200])
vb.addItem(neuron_1)
vb.addItem(neuron_2)
vb.addItem(bg)
## handles scaling both vertically and horizontally
neuron_1.addScaleHandle([1, 1], [0, 0])
neuron_2.addScaleHandle([1, 1], [0, 0])
bg.addScaleHandle([1,1], [0,0])
neuron_1.addScaleHandle([0, 0], [1, 1])
neuron_2.addScaleHandle([0, 0], [1, 1])
bg.addScaleHandle([0,0], [1,1])

# Camera Player
ptr = 0
lastTime = ptime.time()
fps = None
def update():
    global ui, ptr, lastTime, fps, img

    camera.setExpoTime(ui.spinBox_expo.value()/1000.000)

    if  ui.radioButton_capture.isChecked():
        img.setImage(camera.seq_capt(ui.spinBox_Interval.value()/1000.00))
    elif ui.radioButton_pre.isChecked():
        img.setImage(camera.live())
    else:
        img.setImage(camera.live())
    # Transfer the postion and color of ROI and edge
    if ui.checkBox_sti.isChecked():
        ui.pushButton.clicked.connect(
        map.updateRoi(
            bg.parentBounds().getRect(),
            neuron_1.parentBounds().getRect(),
            ui.spinBox_1_red.value(), ui.spinBox_1_blue.value(),
            neuron_2.parentBounds().getRect(),
            ui.spinBox_2_red.value(), ui.spinBox_2_blue.value()
            )
        )
    else:
        map.setRoi(0,1024,0,768,0,0)

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
