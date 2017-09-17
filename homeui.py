# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'homeui.ui'
#
# Created by: PyQt5 UI code generator 5.6
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(1055, 701)
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap("p27.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        MainWindow.setWindowIcon(icon)
        MainWindow.setAutoFillBackground(True)
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setEnabled(True)
        self.centralwidget.setAutoFillBackground(True)
        self.centralwidget.setObjectName("centralwidget")
        self.gridLayoutWidget = QtWidgets.QWidget(self.centralwidget)
        self.gridLayoutWidget.setGeometry(QtCore.QRect(-1, -1, 671, 701))
        self.gridLayoutWidget.setObjectName("gridLayoutWidget")
        self.gridLayout = QtWidgets.QGridLayout(self.gridLayoutWidget)
        self.gridLayout.setContentsMargins(0, 0, 0, 0)
        self.gridLayout.setObjectName("gridLayout")
        self.graphicsView = GraphicsView(self.gridLayoutWidget)
        self.graphicsView.setEnabled(True)
        self.graphicsView.setInteractive(False)
        self.graphicsView.setObjectName("graphicsView")
        self.gridLayout.addWidget(self.graphicsView, 0, 0, 1, 1)
        self.gridLayoutWidget_2 = QtWidgets.QWidget(self.centralwidget)
        self.gridLayoutWidget_2.setGeometry(QtCore.QRect(730, 150, 281, 146))
        self.gridLayoutWidget_2.setObjectName("gridLayoutWidget_2")
        self.gridLayout_2 = QtWidgets.QGridLayout(self.gridLayoutWidget_2)
        self.gridLayout_2.setContentsMargins(0, 0, 0, 0)
        self.gridLayout_2.setObjectName("gridLayout_2")
        self.label_3 = QtWidgets.QLabel(self.gridLayoutWidget_2)
        self.label_3.setObjectName("label_3")
        self.gridLayout_2.addWidget(self.label_3, 4, 0, 1, 1)
        self.label_2 = QtWidgets.QLabel(self.gridLayoutWidget_2)
        self.label_2.setObjectName("label_2")
        self.gridLayout_2.addWidget(self.label_2, 2, 0, 1, 1)
        self.spinBox_Interval = QtWidgets.QSpinBox(self.gridLayoutWidget_2)
        self.spinBox_Interval.setMinimum(0)
        self.spinBox_Interval.setMaximum(99999)
        self.spinBox_Interval.setSingleStep(10)
        self.spinBox_Interval.setObjectName("spinBox_Interval")
        self.gridLayout_2.addWidget(self.spinBox_Interval, 2, 1, 1, 1)
        self.spinBox_3 = QtWidgets.QSpinBox(self.gridLayoutWidget_2)
        self.spinBox_3.setMinimum(1)
        self.spinBox_3.setObjectName("spinBox_3")
        self.gridLayout_2.addWidget(self.spinBox_3, 4, 1, 1, 1)
        self.label = QtWidgets.QLabel(self.gridLayoutWidget_2)
        self.label.setObjectName("label")
        self.gridLayout_2.addWidget(self.label, 1, 0, 1, 1)
        self.spinBox_expo = QtWidgets.QSpinBox(self.gridLayoutWidget_2)
        self.spinBox_expo.setMinimum(3)
        self.spinBox_expo.setMaximum(30000)
        self.spinBox_expo.setSingleStep(10)
        self.spinBox_expo.setProperty("value", 40)
        self.spinBox_expo.setObjectName("spinBox_expo")
        self.gridLayout_2.addWidget(self.spinBox_expo, 1, 1, 1, 1)
        self.horizontalLayoutWidget = QtWidgets.QWidget(self.centralwidget)
        self.horizontalLayoutWidget.setGeometry(QtCore.QRect(730, 20, 197, 80))
        self.horizontalLayoutWidget.setObjectName("horizontalLayoutWidget")
        self.horizontalLayout = QtWidgets.QHBoxLayout(self.horizontalLayoutWidget)
        self.horizontalLayout.setContentsMargins(0, 0, 0, 0)
        self.horizontalLayout.setObjectName("horizontalLayout")
        self.radioButton = QtWidgets.QRadioButton(self.horizontalLayoutWidget)
        self.radioButton.setObjectName("radioButton")
        self.horizontalLayout.addWidget(self.radioButton)
        self.radioButton_capture = QtWidgets.QRadioButton(self.horizontalLayoutWidget)
        self.radioButton_capture.setObjectName("radioButton_capture")
        self.horizontalLayout.addWidget(self.radioButton_capture)
        MainWindow.setCentralWidget(self.centralwidget)

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "MainWindow"))
        self.label_3.setText(_translate("MainWindow", "Number"))
        self.label_2.setText(_translate("MainWindow", "Interval[ms]"))
        self.label.setText(_translate("MainWindow", "Exposure[ms]"))
        self.radioButton.setText(_translate("MainWindow", "Preview"))
        self.radioButton_capture.setText(_translate("MainWindow", "Capture"))

from pyqtgraph import GraphicsView, SpinBox
