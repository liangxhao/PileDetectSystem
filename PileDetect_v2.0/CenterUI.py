from widgets.ImageView import ImageView
from widgets.ImageShowCircle import ImageShowCircle
from widgets.ImagePoints import ImagePoints
from widgets.ImageCicles import ImageCircles
from widgets.FileListView import FileListView
from widgets.PointsListView import PointsListView
from widgets.CirclesListView import CirclesListView
from widgets.ImageComposite import ImageComposite
from PyQt5.QtWidgets import QHBoxLayout, QWidget,QSplitter,QTabWidget,QMessageBox
from PyQt5.QtCore import Qt
from PyQt5.QtGui import QImage

from PIL import ImageQt,Image
import os

class CenterUI(QWidget):
    def __init__(self):
        super().__init__()
        self.__initVariable()
        self.__initUI()




    def __initUI(self):
        self.tabwidget=QTabWidget()

        self.fileViewer=FileListView()#图片列表
        self.imageViewer=ImageView()#显示图像      
        
        self.tabwidget.addTab(self.fileViewer,'图片列表')
        self.tabwidget.setTabPosition(QTabWidget.South)


        self.splitter = QSplitter(Qt.Horizontal)
        self.splitter.addWidget(self.tabwidget)
        self.splitter.addWidget(self.imageViewer)

        #设置初始每个Widget的大小
        width=self.width()
        ratio=[0.2,0.8]
        self.splitterSize=[width*ratio[0],width*ratio[1]]
        self.__setSplitterSize()


        hbox=QHBoxLayout()
        hbox.addWidget(self.splitter)
        
        self.setLayout(hbox)
        self.fileViewer.selectImage.connect(self.__showImage)
        self.splitter.splitterMoved.connect(self.__getSplitterSize)


    def __initVariable(self):
        # self.currentImageFile=None

        #0普通模式
        #1控制点模式
        #2圆模式
        self.Mode=0
        self.pLister=None
        self.cLister=None

    def __setSplitterSize(self):
        self.splitter.setSizes(self.splitterSize)


    def __getSplitterSize(self,pos,index):
        self.splitterSize=self.splitter.sizes()

    #设置模式
    def setMode(self,mode):
        self.Mode=mode
        self.imageViewer.clearImage()



    def addPointListView(self):
        if self.Mode!=1:
            return
        
        num=self.tabwidget.count()
        if num==2:
            self.tabwidget.removeTab(1)
        
        self.pLister=PointsListView()
        self.tabwidget.addTab(self.pLister,'控制点列表')


    def deletePointListView(self):
        if self.Mode==1:
            raise RuntimeError('can not delete PointsListView when it working')

        num=self.tabwidget.count()
        if num==2:
            self.tabwidget.removeTab(1)
            self.pLister=None


    def addCircleListView(self):
        if self.Mode!=2:
            return
        
        num=self.tabwidget.count()
        if num==2:
            self.tabwidget.removeTab(1)
        
        self.cLister=CirclesListView()
        self.tabwidget.addTab(self.cLister,'基桩列表')



    def deleteCircleListView(self):
        if self.Mode==2:
            raise RuntimeError('can not delete CirclesListView when it working')

        num=self.tabwidget.count()
        if num==2:
            self.tabwidget.removeTab(1)
            self.cLister=None
    





    #显示图像
    def __showImage(self,idx,file):
        if self.Mode==0:
            self.__showNormalImage(file)
        elif  self.Mode==1:
            self.__showImageCPoints(idx,file)
        elif self.Mode==2:
            self.__showImageCircles(idx,file)

   


    def __showNormalImage(self,file):
        if not os.path.exists(file):
            QMessageBox.information(self,'失败','图像不存在，请检查路径')
            return
        # self.currentImageFile=file

        frame=Image.open(file)
        img=ImageQt.ImageQt(frame).convertToFormat(QImage.Format_RGB888)

        if type(self.imageViewer) is not ImageView:
            #删除掉旧控件
            viewer=self.splitter.widget(1)
            viewer.deleteLater()
            #设置当前显示图像的控件
            del self.imageViewer
            self.imageViewer=ImageView()
            self.splitter.insertWidget(1,self.imageViewer)
            self.__setSplitterSize()


        self.imageViewer.clearImage()
        self.imageViewer.setImage(img)


    def showImportCircle(self,names,loaction,radius):
        if len(names)==0:
            return
        
        if type(self.imageViewer) is not ImageShowCircle:
            #删除掉旧控件
            viewer=self.splitter.widget(1)
            # viewer.setParent(self)
            viewer.deleteLater()
            #设置当前显示图像的控件
            del self.imageViewer
            self.imageViewer=ImageShowCircle()
            self.splitter.insertWidget(1,self.imageViewer)
            self.__setSplitterSize()

        self.imageViewer.setCircles(names,loaction,radius)


    def __showImageCPoints(self,idx,file):
        if not os.path.exists(file):
            return
        #读取图像
        frame=Image.open(file)
        img=ImageQt.ImageQt(frame).convertToFormat(QImage.Format_RGB888)

        #删除掉旧控件
        viewer=self.splitter.widget(1)
        viewer.deleteLater()
        #设置当前显示图像的控件
        del self.imageViewer
        self.imageViewer=ImagePoints()
        self.splitter.insertWidget(1,self.imageViewer)
        self.__setSplitterSize()

        self.imageViewer.setImage(idx,img)


    def __showImageCircles(self,idx,file):
        if not os.path.exists(file):
            return
        #读取图像
        frame=Image.open(file)
        img=ImageQt.ImageQt(frame).convertToFormat(QImage.Format_RGB888)

        #删除掉旧控件
        viewer=self.splitter.widget(1)
        viewer.deleteLater()
        #设置当前显示图像的控件
        del self.imageViewer
        self.imageViewer=ImageCircles()
        self.splitter.insertWidget(1,self.imageViewer)
        self.__setSplitterSize()

        self.imageViewer.setImageWithIdx(idx,img)



    def showImportComposite(self):
        #删除掉旧控件
        viewer=self.splitter.widget(1)
        # viewer.setParent(self)
        viewer.deleteLater()
        #设置当前显示图像的控件
        del self.imageViewer
        self.imageViewer=ImageComposite()
        self.splitter.insertWidget(1,self.imageViewer)
        self.__setSplitterSize()

        

        
        


