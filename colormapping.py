#!/usr/bin/python
# -*- coding: UTF-8 -*-

"""
Programe to light the C.elegans
"""

import sys
import os
from PyQt5.QtCore import Qt, QTimer
from PyQt5.QtGui import QPainter,QColor, QPen
from PyQt5.QtWidgets import(QWidget, QHBoxLayout, QFrame, QPushButton,
        QApplication, QGridLayout, QLineEdit, QCheckBox, QSlider, QLabel,
        QSplitter)
import numpy as np
################################################################################
#                                                                              #
#                              1. Control Launch Pad                           #
#                                                                              #
################################################################################
class mainWindow(QWidget):
    def __init__(self, parent=None):
        super(mainWindow, self).__init__(parent)
        self.setWindowTitle('Control Pad  -- iGEM_ShenZhen 2017')
        self.setGeometry(0, 0, 1900, 1200)
        self.setAutoFillBackground(True) # Set Window background be black
        p = self.palette()
        p.setColor(self.backgroundRole(),QColor(0,0,0))
        self.setPalette(p)
        mainbox = QHBoxLayout(self)

        # open Camera and set map to fullscreen and black backgram
        preview_button   = QPushButton(u'Preview', self)
        #self.canvas = canvas()
        #self.canvas.show()
        #preview_button.clicked.connect(os.system('python maplive.py -pre'))       
        #preview_button.clicked.connect(self.canvas.showFullScreen)


        preview_button.setToolTip('Fullscreen map and open camera')

        # Power on the projector and projecte image
        stimulate_button = QPushButton('Stimulate',self)  # Check the projector
        stimulate_button.setToolTip('Projecte image')

        # Start Capture
        capture_button   = QPushButton('Capture',self)
        capture_button.setToolTip('Capture the image sequence')

        control_box = QHBoxLayout()
        control_box.addWidget(preview_button)
        control_box.addWidget(stimulate_button)
        control_box.addWidget(capture_button)

        roi_box = QGridLayout()
        roi_1 = QPushButton("ROT 1") # Start pyqtgraph ROI
        roi_box.addWidget(QLineEdit("Red"),0,2)
        roi_box.addWidget(QPushButton("ROI 1"),0,0)
        roi_box.addWidget(QCheckBox("Red"),0,1)
        roi_box.addWidget(QCheckBox("Blue"),0,3)
        roi_box.addWidget(QLineEdit("Blue"),0,4) #Send to map.addroi

        roi_2 = QPushButton("ROT 2") #        p = self.palette()
        p.setColor(self.backgroundRole(),QColor(0,0,0))
        self.setPalette(p)
        roi_box.addWidget(QPushButton("ROI 2"),1,0)
        roi_box.addWidget(QCheckBox("Red"),1,1)
        roi_box.addWidget(QLineEdit("Red"),1,2)
        roi_box.addWidget(QCheckBox("Blue"),1,3)
        roi_box.addWidget(QLineEdit("Blue"),1,4)


        camera_box = QGridLayout()
        camera_box.setSpacing(20)
        camera_box.addWidget(QLabel("Expose_lab"),0,0)
        camera_box.addWidget(QLineEdit(),0,1)
        camera_box.addWidget(QSlider(Qt.Horizontal),0,2)
        camera_box.addWidget(QLabel("Capture Seq"),1,0)
        camera_box.addWidget(QLineEdit(),1,1)
        camera_box.addWidget(QSlider(Qt.Horizontal),1,2)
        camera_box.addWidget(QPushButton("Normal"),2,0)

        video_frame = QFrame(self)
        video_frame.setFrameShape(QFrame.StyledPanel)

        control_frame = QFrame(self)
        control_frame.setFrameShape(QFrame.StyledPanel)
        control_frame.setLayout(control_box)

        roi_frame = QFrame(self)
        roi_frame.setFrameShape(QFrame.StyledPanel)
        roi_frame.setLayout(roi_box)

        camera_frame = QFrame(self)
        camera_frame.setFrameShape(QFrame.StyledPanel)
        camera_frame.setLayout(camera_box)


        splitter1 = QSplitter(Qt.Vertical)
        splitter1.addWidget(control_frame)
        splitter1.addWidget(roi_frame)
        splitter1.addWidget(camera_frame)

        splitter2 = QSplitter(Qt.Horizontal)
        splitter2.addWidget(video_frame)
        splitter2.addWidget(splitter1)


        mainbox.addWidget(splitter2)

        self.setLayout(mainbox)


###############################################################################
#                                                                             #
#                           2.ROI Pad                                         #
#                                                                             #
###############################################################################





#####################################################





if __name__ == '__main__':
    app = QApplication(sys.argv)
    mainwindow = mainWindow()
    mainwindow.show()
    app.exec_()
