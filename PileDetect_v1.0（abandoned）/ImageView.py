from PyQt5.QtWidgets import QGraphicsView,QGraphicsScene,QMainWindow,QGraphicsLineItem,QGraphicsTextItem,\
    QGraphicsItemGroup,QGraphicsEllipseItem
from PyQt5.QtCore import Qt, QRectF,pyqtSignal
from PyQt5.QtGui import QImage, QPixmap,QTransform,QPen,QColor,QFont
import gc
import copy

class ImageView(QGraphicsView):


    rightMouseButtonPressed = pyqtSignal(float, float)


    def __init__(self,parent=None):
        super(ImageView,self).__init__(parent)

        self.scene=QGraphicsScene()
        self.setScene(self.scene)
        self._pixmapHandle = None
        self.aspectRatioMode = Qt.KeepAspectRatio

        # self.setDragMode(QGraphicsView.ScrollHandDrag)


        self.setVerticalScrollBarPolicy(Qt.ScrollBarAlwaysOff)
        self.setHorizontalScrollBarPolicy(Qt.ScrollBarAlwaysOff)


        self.rightMouseButtonPressed.connect(self.handleRightClick)

        self.zoom=-1
      

    def hasImage(self):
        return self._pixmapHandle is not None


    def clearImage(self):
        """ Removes the current image pixmap from the scene if it exists.
        """
        if self.hasImage():
            self.scene.removeItem(self._pixmapHandle)
            self._pixmapHandle = None
            self.zoom=-1
            self.scene.clear()



    def pixmap(self):
        """ Returns the scene's current image pixmap as a QPixmap, or else None if no image exists.
        :rtype: QPixmap | None
        """
        if self.hasImage():
            return self._pixmapHandle.pixmap()
        return None



    def image(self):
        """ Returns the scene's current image pixmap as a QImage, or else None if no image exists.
        :rtype: QImage | None
        """
        if self.hasImage():
            return self._pixmapHandle.pixmap().toImage()
        return None

    def setImage(self, image):
        """ Set the scene's current image pixmap to the input QImage or QPixmap.
        Raises a RuntimeError if the input image has type other than QImage or QPixmap.
        :type image: QImage | QPixmap
            """
        if type(image) is QPixmap:
            pixmap = image
        elif type(image) is QImage:
            pixmap = QPixmap.fromImage(image)
        else:
            raise RuntimeError("ImageViewer.setImage: Argument must be a QImage or QPixmap.")
        if self.hasImage():
            self._pixmapHandle.setPixmap(pixmap)
        else:
            self._pixmapHandle = self.scene.addPixmap(pixmap)
        self.setSceneRect(QRectF(pixmap.rect()))  # Set scene size to image size.
        self.updateViewer()
    


    def updateViewer(self):
        """ Show current zoom (if showing entire image, apply current aspect ratio mode).
        """
        if not self.hasImage():
            return
        if self.zoom<0:
            self.fitInView(self.sceneRect(), self.aspectRatioMode)
            self.zoom=self.size().width()/self.scene.width()
        else:
            self.setTransform(QTransform().scale(self.zoom, self.zoom))


            

    def wheelEvent(self,event):
        moose=event.angleDelta().y()

        if moose>0:
            self.zoomIn()
        elif moose<0:
            self.zoomOut()


    def zoomIn(self):
        self.zoom *= 1.05
        self.updateViewer()

    def zoomOut(self):
        self.zoom /= 1.05
        self.updateViewer()   


    def mousePressEvent(self, event):

        if event.button() == Qt.RightButton:

            scenePos = self.mapToScene(event.pos())
            self.rightMouseButtonPressed.emit(scenePos.x(), scenePos.y())

        elif event.button() == Qt.LeftButton:
            self.setDragMode(QGraphicsView.ScrollHandDrag)
            
        QGraphicsView.mousePressEvent(self, event)



    def mouseReleaseEvent(self, event):

        QGraphicsView.mouseReleaseEvent(self, event)

        if event.button() == Qt.LeftButton:
            self.setDragMode(QGraphicsView.NoDrag)
             

    
    def handleRightClick(self,x, y):
        # row = int(y)
        # col = int(x)
        # print("Clicked on image pixel (row="+str(row)+", column="+str(col)+")")
        pass





class ImageViewPoint(ImageView):
    pointSelect=pyqtSignal(float,float)

    def __init__(self,parent=None):
        super(ImageViewPoint,self).__init__(parent)
        self.isSelecting=False  #是否正在进行选点

        #保存选择的控制点 (id,point)
        self.pointsItem=dict()
        #(id,[x,y])
        self.pointsXYItem=dict()

        self.pointsName=[]

        self.currentID=-1
        self.showPixel=False

        self.canSwitchImage=True #是否允许改变当前显示的图像
        
    #设置最多选择的点数和点名
    def setPointsName(self,names):
        self.hideAllPoints()
        self.deleteAllData()
        self.pointsName=copy.deepcopy(names)
        num=len(names)

        for i in range(num):
            self.pointsItem[i]=None
            self.pointsXYItem[i]=None


    def setImage(self,image):
        if not self.canSwitchImage:
            return

        self.deleteAllData()
        super().clearImage()
        super().setImage(image)
        


    #当前正在编辑的点号. 0开始
    def setPointID(self,currentID):
        self.currentID=currentID

    #是否允许在图上选点
    def setCanSelect(self,flag=False):
        self.isSelecting=flag


    #计算画笔尺寸
    def __setPen(self):
        self.lineRadius=15*max(1/self.zoom,1)
        fontSize=15*max(1/self.zoom,1)

        self.pen=QPen()
        self.pen.setBrush(Qt.red)
        self.pen.setWidth(0.1)

        self.font=QFont()
        self.font.setPointSize(fontSize)

    #缩放时更新图像显示
    def updateViewer(self):
        super().updateViewer()

        if not self.showPixel:
            return


        self.__setPen()
        #更新每个点的显示
        for i in self.pointsItem:
            if self.pointsItem[i] is not None:
                (x,y)=self.pointsXYItem[i]
                self.__drawPoint(x,y,i)


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

        text.setPlainText(self.pointsName[index])
        text.setPos(x,y)

        #放到组中
        pointGroup=QGraphicsItemGroup()
        pointGroup.addToGroup(line1)
        pointGroup.addToGroup(line2)
        pointGroup.addToGroup(text)

        #显示
        if self.pointsItem[index] is not None:
            self.scene.removeItem(self.pointsItem[index])

        #保存到字典
        self.pointsItem[index]=pointGroup
        #显示该点
        self.scene.addItem(self.pointsItem[index])
        
        


    def handleRightClick(self,x, y):
        if not self.isSelecting or self.currentID<0:
            return

        if not self.showPixel:
            return

        self.__setPen()
        self.__drawPoint(x,y,self.currentID)
        self.pointsXYItem[self.currentID]=[x,y]

        self.pointSelect.emit(x,y)



    #{id,[x,y]} dict
    def showAllPoints(self,points):
        self.showPixel=True
        num=len(points)
        if num==0:
            return
        
        self.__setPen()
        for i,point in enumerate(points):    
            if point[0]=='':
                continue
            self.__drawPoint(point[0],point[1],i)
            self.pointsXYItem[i]=point

        

    #隐藏所有显示的点
    def hideAllPoints(self):
        self.showPixel=False
        for i in range(len(self.pointsItem)):
            if self.pointsItem[i] is not None:
                self.scene.removeItem(self.pointsItem[i])
                del(self.pointsItem[i])
                self.pointsItem[i]=None

        gc.collect()

    #删除某点的数据与显示
    def deletePoint(self,index):       
        if self.pointsItem[index] is not None:
            self.scene.removeItem(self.pointsItem[index])
            self.pointsItem[index]=None
            self.pointsXYItem[index]=None


    #删除掉所有数据
    def deleteAllData(self):
        self.currentID=-1
        self.showPixel=False
        self.pointsItem.clear()
        self.pointsXYItem.clear()
        self.pointsName.clear()
        gc.collect()



class ImageViewCircle(ImageView):
    def __init__(self,parent=None):
        super(ImageViewCircle,self).__init__(parent)


    def setCircles(self,names,points,radius):
        if len(points)!=len(names):
            return

        if len(points)==0:
            return

        self.pointsName=names
        self.pointsXYItem=points
        self.radius=radius

        #计算显示范围
        x=[p[0] for p in points]
        xMax=max(x)
        xMin=min(x)

        y=[p[1] for p in points]
        yMax=max(y)
        yMin=min(y)
    
        rMax=max(radius)
        
        width=xMax-xMin+2*rMax
        height=yMax-yMin+2*rMax

        rect=QRectF(xMin,yMin,width,height)

        self.setSceneRect(rect)  # Set scene size to image size.

        self.__showCircle()
        self.updateViewer()

    def updateViewer(self):
        # if not self.hasImage():
        #     return
        if self.zoom<0:
            self.fitInView(self.sceneRect(), self.aspectRatioMode)
            self.zoom=self.size().width()/self.scene.width()
        else:
            self.setTransform(QTransform().scale(self.zoom, self.zoom))


    def __showCircle(self):
        ratio=3

        pen=QPen()
        pen.setBrush(Qt.red)
        pen.setWidth(0.01)

        font=QFont()
        font.setPointSize(200*ratio)

        self.scene.clear()


        for i in range(len(self.pointsXYItem)):
            (x,y)=self.pointsXYItem[i]
            r=self.radius[i]*ratio

            circleItem=QGraphicsEllipseItem(x-r,y-r,2*r,2*r)
            circleItem.setPen(pen)

            self.scene.addItem(circleItem)

            strItem=QGraphicsTextItem()
            strItem.setDefaultTextColor(Qt.blue)
            strItem.setFont(font)

            strItem.setPlainText(self.pointsName[i])
            strItem.setPos(x,y)

            self.scene.addItem(strItem)



class ImageViewSegment(ImageView):
    def __init__(self,parent=None):
        super(ImageViewSegment,self).__init__(parent)

        self.__setDrawParms()
        self.pointsItem=[]


    def __setDrawParms(self):
        self.pen=QPen()
        self.pen.setBrush(Qt.red)
        self.pen.setWidth(0.1)

        self.lineRadius=10

        fontSize=50

        self.font=QFont()
        self.font.setPointSize(fontSize)


    def setNames(self,names):
        self.names=names


    def addCircle(self,index,points,r):
        (x,y)=points

        circleItem=QGraphicsEllipseItem(x-r,y-r,2*r,2*r)
        circleItem.setPen(self.pen)

        # self.scene.addItem(circleItem)

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

        text.setPlainText(str(self.names[index]))
        text.setPos(x,y)

        #放到组中
        pointGroup=QGraphicsItemGroup()
        pointGroup.addToGroup(line1)
        pointGroup.addToGroup(line2)
        pointGroup.addToGroup(text)
        pointGroup.addToGroup(circleItem)

        #显示该点
        self.scene.addItem(pointGroup)

        self.pointsItem.append(pointGroup)



    def hideAllItem(self):
        for item in self.pointsItem:
            self.scene.removeItem(item)

        self.pointsItem.clear()


    def showData(self,indexList,pointsList,radiusList):
        for (index,points,radius) in zip(indexList,pointsList,radiusList):
            self.addCircle(index,points,radius)







            






