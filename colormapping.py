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
#import maptest

#QtGui.QApplication.setGraphicsSystem('raster')
app = QtGui.QApplication([])

win = QtGui.QMainWindow()
ui = homeui.Ui_MainWindow() # set ui
ui.setupUi(win)
win.setWindowTitle('Light for Life -- iGEM 2017')
win.show()
#map1=maptest()
#win.addWindow(map1)
#ui.graphicsView.useOpenGL()
vb = pg.ViewBox()
ui.graphicsView.setCentralItem(vb)
vb.setAspectLocked()
img = pg.ImageItem()
vb.addItem(img)

# Camera setting and Image Store

# ROI setting
rois = []
for i in [0,1,2,3]:
    y0=700+i*300
    rois.append(pg.RectROI([1024,y0],[100,100]))
    vb.addItem(rois[i])
bg = pg.LineROI([0,0], [0,-2000],width=5)
vb.addItem(bg)

# Camera Player
ptr = 0
lastTime = ptime.time()
fps = None
def update():
    global ui, ptr, lastTime, fps, img


    # Transfer the orign postion and color of ROI and edge
    if ui.pushButton_roi.isChecked():
        wormmap = np.zeros((4,6))
        for i in [0,1,2,3]:
            #print np.transpose(rois[i].parentBounds().getRect())
            wormmap[i, 0:4] = np.transpose(rois[i].parentBounds().getRect())
        wormmap[0, 4:6]= [ui.spinBox_0_red.value(),ui.spinBox_0_blue.value()]
        wormmap[1, 4:6]= [ui.spinBox_1_red.value(),ui.spinBox_1_blue.value()]
        wormmap[2, 4:6]= [ui.spinBox_2_red.value(),ui.spinBox_2_blue.value()]
        wormmap[3, 4:6]= [ui.spinBox_3_red.value(),ui.spinBox_3_blue.value()]

        map.setRoi(bg.parentBounds().getRect(),wormmap)

    # Update Image
    if ui.checkBox_sti.isChecked():
        map.updateImage()

    camera.setExpoTime(ui.spinBox_expo.value()/1000.000)
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
