from .ImageView import ImageView
from PyQt5.QtWidgets import QGraphicsView,QGraphicsScene,QGraphicsLineItem,QGraphicsTextItem,\
    QGraphicsItemGroup,QGraphicsEllipseItem

from PyQt5.QtCore import Qt, QRectF
from PyQt5.QtGui import QTransform,QPen,QColor,QFont


class ImageShowCircle(ImageView):
    def __init__(self, parent=None):
        super(ImageShowCircle, self).__init__()

    def setCircles(self, names, points, radius):
        if len(points) != len(names):
            return

        if len(points) == 0:
            return

        self.pointsName = names
        self.pointsXYItem = points
        self.radius = radius

        # 计算显示范围
        x = [p[0] for p in points]
        xMax = max(x)
        xMin = min(x)

        y = [p[1] for p in points]
        yMax = max(y)
        yMin = min(y)

        rMax = max(radius)

        width = xMax - xMin + 2 * rMax
        height = yMax - yMin + 2 * rMax

        rect = QRectF(xMin, yMin, width, height)

        self.setSceneRect(rect)  # Set scene size to image size.

        self.__showCircle()
        self.updateViewer()

    def updateViewer(self):
        if self.zoom < 0:
            self.fitInView(self.sceneRect(), self.aspectRatioMode)
            self.zoom = self.size().width() / self.scene.width()
        else:
            self.setTransform(QTransform().scale(self.zoom, self.zoom))

    def __showCircle(self):
        pen = QPen()
        pen.setBrush(Qt.red)
        pen.setWidth(0.01)

        font = QFont()
        font.setPointSize(300)

        self.scene.clear()

        for i in range(len(self.pointsXYItem)):
            (x, y) = self.pointsXYItem[i]
            r = self.radius[i]

            circleItem = QGraphicsEllipseItem(x - r, y - r, 2 * r, 2 * r)
            circleItem.setPen(pen)

            self.scene.addItem(circleItem)

            strItem = QGraphicsTextItem()
            strItem.setDefaultTextColor(Qt.blue)
            strItem.setFont(font)

            strItem.setPlainText(self.pointsName[i])
            strItem.setPos(x, y)

            self.scene.addItem(strItem)

