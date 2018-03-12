from .ImageDragCircleView import ImageDragCircleView
from PyQt5.QtGui import QPen,QColor,QFont

from PyQt5.QtWidgets import QGraphicsScene,QGraphicsLineItem,QGraphicsTextItem,\
    QGraphicsItemGroup,QGraphicsEllipseItem
from PyQt5.QtCore import Qt, QRectF,pyqtSignal

class ImageCircles(ImageDragCircleView):
    # pointSelectCoord=pyqtSignal(float,float)
    # 发送圆的半径和圆心
    circleSelectCoord=pyqtSignal(float,float,float)

    def __init__(self):
        super(ImageCircles,self).__init__()
        self.__initVariable()


    def __initVariable(self):
        # self.allNames=dict()   #所有的，名 (id,name)
        # self.pointsLoc=dict() #在选的，坐标 (id,point)

        self.allIdx=[] #所有点的标号
        # self.hasEditedIdx=[]#已经标记过的标号

        self.circlesGrap=dict() #绘图项

        self.currentID=-1#当前正在编辑的点
        self.imageID=-1#图像的id号

        self.__setDrawParms()


    def __setDrawParms(self):
        #备选点
        self.__Apen=QPen()
        self.__Apen.setBrush(Qt.blue)
        self.__Apen.setWidth(2)

        self.__lineRadius=2

        self.__Afont=QFont()
        self.__Afont.setPointSize(70)

        self.__AtextColor=Qt.green
        #选择点
        self.__Spen=QPen()
        self.__Spen.setBrush(Qt.red)
        self.__Spen.setWidth(2)


    def setImageWithIdx(self,idx,image):
        super().setImage(image)
        self.imageID=idx


    def setAlternativeCircles(self, ids, names, points, radiuses):
        # 保存所有需要绘制基桩的id
        self.allIdx=ids

        for idx,name,point,radius in zip(ids,names,points,radiuses):
            # self.allNames[idx]=name
            self.__drawCircle(idx,name,point,radius,self.__Apen,self.__Afont,self.__AtextColor)




    def __drawCircle(self,idx,name,point,radius,pen,font=None,textColor=None):
        (x,y)=point
        pen.setWidth(2)
        circleItem=QGraphicsEllipseItem(x-radius,y-radius,2*radius,2*radius)
        circleItem.setPen(pen)

        pen.setWidth(0.1)

        #横线
        line1=QGraphicsLineItem()
        line1.setPen(pen)
        line1.setLine(x-self.__lineRadius,y,x+self.__lineRadius,y)

        #竖线
        line2=QGraphicsLineItem()
        line2.setPen(pen)
        line2.setLine(x,y-self.__lineRadius,x,y+self.__lineRadius)    

        #放到组中
        circleGroup=QGraphicsItemGroup()
        circleGroup.addToGroup(circleItem)
        circleGroup.addToGroup(line1)
        circleGroup.addToGroup(line2)

        if font is not None:
            #文字说明
            text=QGraphicsTextItem()
            text.setDefaultTextColor(textColor)
            text.setFont(font)

            text.setPlainText(name)
            text.setPos(x,y)

            circleGroup.addToGroup(text)       
        else:
            if self.circlesGrap[idx] is not None:
                self.scene.removeItem(self.circlesGrap[idx])
            #保存到字典
            self.circlesGrap[idx]=circleGroup

        self.scene.addItem(circleGroup)


    # 绘制之前已经已经标记过的基桩（批量）
    def setSelectedCircles(self, ids, points, radiuses):
        for idx,point,radius in zip(ids, points, radiuses):
            # self.pointsLoc[idx]=point
            # self.hasEditedIdx.append(idx)
            self.circlesGrap[idx]=None
            self.__drawCircle(idx,None,point,radius,self.__Spen)

    # 绘制单个基桩
    def setSingleSelectedCircle(self, idx, px,py,pr):
        self.circlesGrap[idx] = None
        self.__drawCircle(idx, None, [px,py], pr, self.__Spen)


    def deleteCircle(self,idx):
        if self.circlesGrap[idx] is not None:
            self.scene.removeItem(self.circlesGrap[idx])

        self.circlesGrap[idx]=None
        # self.pointsLoc[idx]=[None,None]
        # if idx in self.hasEditedIdx:
        #     self.hasEditedIdx.remove(idx)




    #设置当前正在编辑的点
    def setCurrentPoint(self,idx):
        self.currentID=idx
        self.drawing=True

        # if idx not in self.pointsLoc:
        #     self.pointsLoc[idx]=[None,None]
        #     self.circlesGrap[idx]=None
        if idx not in self.circlesGrap:
            self.circlesGrap[idx] = None

    # 清除当前的选择
    def clearSelect(self):
        self.currentID = -1
        self.drawing = False


    # def handleRightClick(self, x, y):
    #     if self.currentID<0:
    #         return
    #     x=round(x,3)
    #     y=round(y,3)
    #
    #     point=[x,y]
    #
    #     self.__drawCircle(self.currentID,None,point,self.__Spen)
    #     self.pointsLoc[self.currentID]=point
    #
    #     self.pointSelectCoord.emit(x,y)


    def handleRightDragCircle(self,x,y,r):
        if self.currentID<0:
            return
        point=[x,y]
        # 绘制
        self.__drawCircle(self.currentID,None,point,r,self.__Spen)



    def handleRightReleaseCircle(self,x,y,r):
        if self.currentID<0:
            return
        # 圆心和半径
        x=round(x,3)
        y=round(y,3)

        point=[x,y]
        r=round(r,3)
        # 绘制
        self.__drawCircle(self.currentID,None,point,r,self.__Spen)

        self.circleSelectCoord.emit(x,y,r)




