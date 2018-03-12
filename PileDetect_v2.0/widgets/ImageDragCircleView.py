from .ImageView import ImageView
from PyQt5.QtCore import Qt
from PyQt5.QtWidgets import QGraphicsView,QGraphicsScene

class ImageDragCircleView(ImageView):
    def __init__(self):
        super(ImageDragCircleView, self).__init__()
        # self.__initVariable()
        self.clickedPoint=None
        # 当前是否正在画圆
        self.drawing = False
        # 鼠标右键是否正在按压
        self.rightpressed=False

    def mousePressEvent(self, event):
        if event.button() == Qt.RightButton:
            scenePos = self.mapToScene(event.pos())
            self.clickedPoint=(scenePos.x(), scenePos.y())
            self.rightpressed=True

        elif event.button() == Qt.LeftButton:
            self.setDragMode(QGraphicsView.ScrollHandDrag)

        QGraphicsView.mousePressEvent(self, event)


    # 没有动态效果。不知道原因，待解决
    def mouseMoveEvent(self,event):
        QGraphicsView.mouseMoveEvent(self, event)
        if self.rightpressed and self.drawing:
            scenePos = self.mapToScene(event.pos())
            point=(scenePos.x(), scenePos.y())

            dist=self.__lengthBetweenPoints(self.clickedPoint,point)

            self.handleRightDragCircle(self.clickedPoint[0],self.clickedPoint[1],dist)



    def mouseReleaseEvent(self, event):

        QGraphicsView.mouseReleaseEvent(self, event)

        if event.button() == Qt.LeftButton:
            self.setDragMode(QGraphicsView.NoDrag)

        elif event.button() == Qt.RightButton and self.drawing:
            scenePos = self.mapToScene(event.pos())
            point = (scenePos.x(), scenePos.y())

            dist = self.__lengthBetweenPoints(self.clickedPoint, point)
            self.handleRightReleaseCircle(self.clickedPoint[0], self.clickedPoint[1], dist)
            self.rightpressed=False




    def __lengthBetweenPoints(self,p1,p2):
        return ((p1[0]-p2[0])**2+(p1[1]-p2[1])**2)**0.5



    def handleRightDragCircle(self,x,y,r):
        pass


    def handleRightReleaseCircle(self,x,y,r):
        pass