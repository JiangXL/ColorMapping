from PyQt5.QtWidgets import QApplication, QWidget

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
