from .ImageView import ImageView
from PyQt5.QtGui import QPen,QColor,QFont

from PyQt5.QtWidgets import QGraphicsScene,QGraphicsLineItem,QGraphicsTextItem,\
    QGraphicsItemGroup,QGraphicsEllipseItem
from PyQt5.QtCore import Qt, QRectF,pyqtSignal

class ImageComposite(ImageView):
    def __init__(self):
        super(ImageComposite,self).__init__()
        self.__initVariable()


    def __initVariable(self):
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
        #选这点
        self.__Spen=QPen()
        self.__Spen.setBrush(Qt.red)
        self.__Spen.setWidth(5)


    def __drawCircle(self,name,point,radius,pen,font=None,textColor=None):
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

        self.scene.addItem(circleGroup)


    #绘制CAD图上的圆
    def setAlternativeCircles(self,names,points,radius):
        for name,point,r in zip(names,points,radius):
            self.__drawCircle(name,point,r,self.__Apen,self.__Afont,self.__AtextColor)

    #绘制用户选择的圆
    def setSelectedCircles(self,points,radius):
        for point,r in zip(points,radius):
            self.__drawCircle(None,point,r,self.__Spen)