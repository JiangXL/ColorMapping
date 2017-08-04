#!/usr/bin/python
# -*- coding: UTF-8 -*-

"""
Programe to light the C.elegans
"""

import sys
from PyQt5.QtCore import Qt
from PyQt5.QtGui import *
from PyQt5.QtWidgets import(QWidget, QHBoxLayout, QFrame, QPushButton,
        QApplication, QGridLayout, QLineEdit, QCheckBox, QSlider, QLabel,
        QSplitter)

################################################################################
#                                                                              #
#                              1. Control Panel                                #
#                                                                              #
################################################################################
class mainWindow(QWidget):
    def __init__(self, parent=None):
        super(mainWindow, self).__init__(parent)
        self.setWindowTitle('Launch Pad')
        self.setGeometry(600, 600, 900, 600)

        preview_button = QPushButton(u'Preview', self)
        stimulate_button = QPushButton('Stimulate',self)
        capture_button = QPushButton('Capture',self)

        control_box = QHBoxLayout()
        control_box.addStretch()
        control_box.addWidget(preview_button)
        control_box.addWidget(stimulate_button)
        control_box.addWidget(capture_button)

        light_box = QGridLayout()
        roi_1 = QPushButton("ROT 1") # connect to a event
        light_box.addWidget(QLineEdit("Red"),0,2)
        light_box.addWidget(QPushButton("ROI 1"),0,0)
        light_box.addWidget(QCheckBox("Red"),0,1)
        light_box.addWidget(QCheckBox("Blue"),0,3)
        light_box.addWidget(QLineEdit("Blue"),0,4)

        roi_2 = QPushButton("ROT 2") # connect to a event
        light_box.addWidget(QPushButton("ROI 1"),1,0)
        light_box.addWidget(QCheckBox("Red"),1,1)
        light_box.addWidget(QLineEdit("Red"),1,2)
        light_box.addWidget(QCheckBox("Blue"),1,3)
        light_box.addWidget(QLineEdit("Blue"),1,4)




        camera_box = QGridLayout()
        camera_box.setSpacing(20)
        camera_box.addWidget(QLabel("Expose_lab"),0,0)
        camera_box.addWidget(QLineEdit(),0,1)
        camera_box.addWidget(QSlider(Qt.Horizontal),0,2)
        camera_box.addWidget(QLabel("Capture Seq"),1,0)
        camera_box.addWidget(QLineEdit(),1,1)
        camera_box.addWidget(QSlider(Qt.Horizontal),1,2)
        camera_box.addWidget(QPushButton("Normal"),2,0)

        main_grid = QGridLayout()
        main_grid.addLayout(control_box,1,3)
        main_grid.addLayout(light_box,2,3)
        main_grid.addLayout(camera_box,3,3)


        self.setLayout(main_grid)

        self.canvas = canvas()
        self.canvas.show()
        preview_button.clicked.connect(self.canvas.showFullScreen)

###############################################################################
#                                                                             #
#                           2.ROI                                             #
#                                                                             #
###############################################################################QPushButton
class canvas(QWidget):
    def __init__(self, parent = None):
        super(canvas, self).__init__(parent)
        self.setGeometry(300, 300, 900, 600)

    def paintEvent(self,e):
        qp=QPainter()
        qp.begin(self)
        self.drawRectangles(qp)
        qp.end()

    def drawRectangles(self, qp):
        color = QColor(0,0,0)
        color.setNamedColor('#d4d4d4')
        qp.setPen(color)

        qp.setBrush(QColor(255,0,0))
        qp.drawRect(10,15,90,60)


        qp.setBrush(QColor(0,0,255))
        qp.drawRect(130,15,90,60)


#def log():


def main():
    app = QApplication(sys.argv)
    mainwindow = mainWindow()
    mainwindow.show()
    app.exec_()


if __name__ == '__main__':
    main()
