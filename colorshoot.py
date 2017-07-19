#!/usr/bin/python
# -*- coding: UTF-8 -*-

"""
Programe to light the C.elegans
"""

import pygame,sys
from PyQt4.QtCore import *
from PyQt4.QtGui import *
from pygame.locals import*

################################################################################
#                                                                              #
#                              1. Control Panel                                #
#                                                                              #
################################################################################
class mainWindow(QWidget):
    def __init__(self, parent=None):
        super(mainWindow, self).__init__(parent)
        button = QPushButton(u'Preview', self)
        self.canvas = canvas()
        self.canvas.show()
        self.connect(button, SIGNAL('clicked()'), self.canvas.showFullScreen)

        #button_sti = QPushButton(u'Stimulate', self)
        #button_Capture= QPushButton(u'Capture', self)


###############################################################################
#                                                                             #
#                           2.ROI                                             #
#                                                                             #
###############################################################################
class canvas(QWidget):
    def __init__(self, parent = None):
        super(canvas, self).__init__(parent)

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
