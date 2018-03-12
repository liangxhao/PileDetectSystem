from .ImageView import ImageView
from PyQt5.QtGui import QPen,QColor,QFont

from PyQt5.QtWidgets import QGraphicsScene,QGraphicsLineItem,QGraphicsTextItem,\
    QGraphicsItemGroup
from PyQt5.QtCore import Qt, QRectF,pyqtSignal

class ImagePoints(ImageView):
    pointSelectCoord=pyqtSignal(float,float)
    def __init__(self):
        super(ImagePoints,self).__init__()
        self.__initVariable()


    def __initVariable(self):
        self.pointsLoc=dict()#存储坐标  ('id',[name,point])
        self.pointsGrap=dict()#存储绘图项

        self.currentID=-1#当前正在编辑的点
        self.imageID=-1#图像的id号


    def setImage(self,idx,image):
        super().setImage(image)
        self.imageID=idx

    #上次已经选择过的点
    def setPoints(self,ids,names,points):
        for idx,name,point in zip(ids,names,points):
            self.pointsLoc[idx]=[name,point]
            self.pointsGrap[idx]=None
            if  None not in point:
                self.__drawPoint(point[0],point[1],idx)


    #增加一个新点
    def addPoint(self,idx,name):
        self.pointsLoc[idx]=[name,[None,None]]
        self.pointsGrap[idx]=None


    def deletePoint(self,idx):
        #存储存储的坐标
        self.pointsLoc.pop(idx)
        #移除显示
        if self.pointsGrap[idx] is not None:
            self.scene.removeItem(self.pointsGrap[idx])
        #删除绘图项
        self.pointsGrap.pop(idx)

        #重置当前编辑项
        if self.currentID==idx:
            self.currentID=-1



    #缩放时更新图像显示
    def updateViewer(self):
        super().updateViewer()

        self.__setPen()
        #更新每个点的显示
        for index in self.pointsLoc:
            item = self.pointsLoc[index]
            if None not in item[1]:
                self.__drawPoint(item[1][0],item[1][1],index)



    #设置当前正在编辑的点
    def setCurrentPoint(self,idx):
        self.currentID=idx


    #计算画笔尺寸
    def __setPen(self):
        self.lineRadius=15*max(1/self.zoom,1)
        fontSize=15*max(1/self.zoom,1)

        self.pen=QPen()
        self.pen.setBrush(Qt.red)
        self.pen.setWidth(0.1)

        self.font=QFont()
        self.font.setPointSize(fontSize)




    def handleRightClick(self, x, y):
        if self.currentID<0:
            return
        x=round(x,3)
        y=round(y,3)

        self.__drawPoint(x,y,self.currentID)
        self.pointsLoc[self.currentID][1]=[x,y]

        self.pointSelectCoord.emit(x,y)



    #在图上画该点
    def __drawPoint(self,x,y,index):
        #横线
        line1=QGraphicsLineItem()
        line1.setPen(self.pen)
        line1.setLine(x-self.lineRadius,y,x+self.lineRadius,y)

        #竖线
        line2=QGraphicsLineItem()
        line2.setPen(self.pen)
        line2.setLine(x,y-self.lineRadius,x,y+self.lineRadius)    

        #文字说明
        text=QGraphicsTextItem()
        text.setDefaultTextColor(Qt.blue)
        text.setFont(self.font)

        text.setPlainText(self.pointsLoc[index][0])
        text.setPos(x,y)

        #放到组中
        pointGroup=QGraphicsItemGroup()
        pointGroup.addToGroup(line1)
        pointGroup.addToGroup(line2)
        pointGroup.addToGroup(text)

        #显示
        if self.pointsGrap[index] is not None:
            self.scene.removeItem(self.pointsGrap[index])

        #保存到字典
        self.pointsGrap[index]=pointGroup
        #显示该点
        self.scene.addItem(self.pointsGrap[index])

