from ImageView import ImageView,ImageViewPoint,ImageViewCircle,ImageViewSegment
from FileListView import FileListView

from PyQt5.QtWidgets import QHBoxLayout, QWidget,QSplitter,QTabWidget
from PyQt5.QtCore import Qt
from PyQt5.QtGui import QImage

from PIL import ImageQt,Image
from PointsListView import PointsListView
import os

class CenterWidget(QWidget):
    def __init__(self):
        super().__init__()
        self.__initUI()
        self.__initVariable()

    def __initUI(self):

        tabwidget=QTabWidget()

        self.fileViewer=FileListView()#图片列表
        self.pointsViewer=PointsListView()#控制点列表
        self.fullViewer=FileListView()#全图列表

        self.imageViewer=ImageView()#显示图像
        
        tabwidget.addTab(self.fileViewer,'待拼接图列表')
        tabwidget.addTab(self.pointsViewer,'控制点列表')
        tabwidget.addTab(self.fullViewer,'全图列表')
        tabwidget.setTabPosition(QTabWidget.South)


        self.splitter = QSplitter(Qt.Horizontal)
        self.splitter.addWidget(tabwidget)
        self.splitter.addWidget(self.imageViewer)

        #设置初始每个Widget的大小
        width=self.width()
        ratio=[0.2,0.8]
        self.splitterSize=[width*ratio[0],width*ratio[1]]
        self.__setSplitterSize()


        hbox=QHBoxLayout()
        hbox.addWidget(self.splitter)
        

        self.setLayout(hbox)


        self.fileViewer.itemDoubleClicked.connect(self.showCurrentImage)
        self.fullViewer.itemDoubleClicked.connect(self.showFullImage)

        self.splitter.splitterMoved.connect(self.__getSplitterSize)


    def __initVariable(self):
        self.currentImageFile=None

    def __setSplitterSize(self):
        self.splitter.setSizes(self.splitterSize)


    def __getSplitterSize(self,pos,index):
        self.splitterSize=self.splitter.sizes()
        



    def showCurrentImage(self,file):
        if not os.path.exists(file):
            return
        
        self.currentImageFile=file

        frame=Image.open(file)
        img=ImageQt.ImageQt(frame).convertToFormat(QImage.Format_RGB888)

        if type(self.imageViewer) is not ImageView:
            #删除掉旧控件
            viewer=self.splitter.widget(1)
            # viewer.setParent(self)
            viewer.deleteLater()
            #设置当前显示图像的控件
            self.imageViewer=ImageView()
            self.splitter.insertWidget(1,self.imageViewer)
            self.__setSplitterSize()


        self.imageViewer.clearImage()
        self.imageViewer.setImage(img)


    def showFullImage(self,file):
        if not os.path.exists(file):
            return
        
        self.currentImageFile=file
        
        frame=Image.open(file)
        img=ImageQt.ImageQt(frame).convertToFormat(QImage.Format_RGB888)

        if type(self.imageViewer) is not ImageViewPoint:
            #删除掉旧控件
            viewer=self.splitter.widget(1)
            # viewer.setParent(self)
            viewer.deleteLater()
            #设置当前显示图像的控件
            self.imageViewer=ImageViewPoint()
            self.splitter.insertWidget(1,self.imageViewer)
            self.__setSplitterSize()

        
        self.imageViewer.clearImage()
        self.imageViewer.setImage(img)


    def showCircleByGround(self,names,loaction,radius):
        if type(self.imageViewer) is not ImageViewCircle:
            #删除掉旧控件
            viewer=self.splitter.widget(1)
            # viewer.setParent(self)
            viewer.deleteLater()
            #设置当前显示图像的控件
            self.imageViewer=ImageViewCircle()
            self.splitter.insertWidget(1,self.imageViewer)
            self.__setSplitterSize()

        self.imageViewer.setCircles(names,loaction,radius)



    def showSegmentImage(self,file):
        if not os.path.exists(file):
            return
        
        self.currentImageFile=file
        
        frame=Image.open(file)
        img=ImageQt.ImageQt(frame).convertToFormat(QImage.Format_RGB888)

        if type(self.imageViewer) is not ImageViewSegment:
            #删除掉旧控件
            viewer=self.splitter.widget(1)
            # viewer.setParent(self)
            viewer.deleteLater()
            #设置当前显示图像的控件
            self.imageViewer=ImageViewSegment()
            self.splitter.insertWidget(1,self.imageViewer)
            self.__setSplitterSize()

        
        self.imageViewer.clearImage()
        self.imageViewer.setImage(img)
