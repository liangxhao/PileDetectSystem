from CenterUI import CenterWidget
from LocationTable import LocationTable
from ImportDxfDialog import ImportDxfDiaolg

from KernelPoints import KernelPoints
from Attribute import Attribute
from DxfTable import DxfTable

from MenuFunction import MenuFuction
from ImageView import ImageViewPoint

from PyQt5.QtWidgets import QMainWindow,QFileDialog,QAction,QFileDialog,QMessageBox,qApp
from PyQt5.QtGui import QIcon
import subprocess
import sys,os


class MainUI(QMainWindow):
    def __init__(self):
        super().__init__()
        self.__initUI()
        self.__initVariable()

    def __initUI(self):
        self.__setupMenuBar()
        self.__setupStatusBar()

        self.centerUi=CenterWidget()
          
        self.setCentralWidget(self.centerUi)
        self.showMaximized()
        
        self.setWindowTitle('UAV')
        

    def __initVariable(self):

        #获取工作目录
        if getattr(sys, 'frozen', False):
            self.programPath = os.path.dirname(sys.executable)
        elif __file__:
            self.programPath = os.path.dirname(__file__)
        
        self.programPath=self.programPath.replace("\\",'/')#主程序所在目录
        
        #设置各种初始文件名
        self.workDir=None        
        self.projectFile=None #工程主文件               
        self.cpKernel=None


    def __setupMenuBar(self):
        #################
        setWorkDirAction=QAction('&新建工程   ',self)
        setWorkDirAction.triggered.connect(self.setWorkDir)
        openWorkAction=QAction('&打开工程文件',self)
        openWorkAction.triggered.connect(self.openWork)
        importImageAction=QAction(QIcon('./res/importImage.png'),'&导入图像   ',self)
        importImageAction.triggered.connect(self.importImages)
        importFullImageAction=QAction('&导入全图  ',self)
        importFullImageAction.triggered.connect(self.importFullImages)
        locationTableAction=QAction(QIcon('./res/table.png'),'&图片坐标',self)
        locationTableAction.triggered.connect(self.lookLocationTable)
        exitAction=QAction('&退出 ',self)
        exitAction.triggered.connect(qApp.quit)

        ##################
        stitchingAction=QAction('  &开始拼接  ',self)
        stitchingAction.triggered.connect(self.stitchingStart)
        resultPictureAction=QAction(QIcon('./res/resultPicture.png'),'  &查看拼接结果  ',self)
        resultPictureAction.triggered.connect(self.lookStitchingResult)


        ###################
        importControlPointAction=QAction('  &导入控制点  ',self)
        importControlPointAction.triggered.connect(self.importControlPoint)

        showControlPointAction=QAction('  &显示/隐藏控制点  ',self)
        showControlPointAction.triggered.connect(self.showControlPoint)
        self.startControlPointAction=QAction('  &开始选择控制点  ',self)
        self.startControlPointAction.triggered.connect(self.startControlPoint)
        self.endControlPointAction=QAction('  &结束选择控制点  ',self)
        self.endControlPointAction.triggered.connect(self.endControlPoint)


        ####################
        importDxfAction=QAction('  &导入CAD图  ',self)
        importDxfAction.triggered.connect(self.importDxf)
        lookDxfTableAction=QAction('  &查看导入的坐标  ',self)
        lookDxfTableAction.triggered.connect(self.lookDxfTable)
        showDxfCircleAction=QAction('  &显示导入图像  ',self)
        showDxfCircleAction.triggered.connect(self.showDxfCircle)


        #####################
        segmentAction=QAction('  &图像分割  ',self)
        segmentAction.triggered.connect(self.segmentObject)
        reportAction=QAction('  &导出报告文件',self)
        reportAction.triggered.connect(self.lookReport)
        showSegmentAction=QAction('  &显示分割图像  ',self)
        showSegmentAction.triggered.connect(self.showSegment)



        ##################
        caliAction=QAction('  &标定参数  ',self)
        caliAction.triggered.connect(self.caliParmsOpen)
        undistortAction=QAction('  &畸变校正  ',self)
        undistortAction.triggered.connect(self.undistortOpen)


        #################
        aboutAction=QAction('  &关于  ',self)
        aboutAction.triggered.connect(self.aboutInfo)

        ####################################
        #菜单栏
        menubar=self.menuBar()

        #第一项菜单
        fileMenu=menubar.addMenu('   &文件   ')
        fileMenu.addAction(setWorkDirAction)
        fileMenu.addAction(openWorkAction)
        fileMenu.addAction(importImageAction)
        fileMenu.addAction(importFullImageAction)
        fileMenu.addAction(locationTableAction)
        fileMenu.addAction(exitAction)
        
        #第二项菜单
        stitchingMenu=menubar.addMenu('   &拼接   ')
        stitchingMenu.addAction(stitchingAction)
        stitchingMenu.addAction(resultPictureAction)

        #第三项菜单
        controlPointMenu=menubar.addMenu('   &控制点   ')
        controlPointMenu.addAction(importControlPointAction)
        controlPointMenu.addAction(showControlPointAction)
        controlPointMenu.addAction(self.startControlPointAction)
        controlPointMenu.addAction(self.endControlPointAction)

        #第四项菜单
        dxfMenu=menubar.addMenu('   &数据   ')
        dxfMenu.addAction(importDxfAction)
        dxfMenu.addAction(lookDxfTableAction)
        dxfMenu.addAction(showDxfCircleAction)

        
        #第五项菜单
        segment=menubar.addMenu('   &分割   ')
        segment.addAction(segmentAction)
        segment.addAction(reportAction)
        segment.addAction(showSegmentAction)




        #第六项菜单
        caliMenu=menubar.addMenu('   &标定   ')
        caliMenu.addAction(caliAction)
        caliMenu.addAction(undistortAction)


        #第七项菜单
        helpMenu=menubar.addMenu('   &帮助   ')
        helpMenu.addAction(aboutAction)

        ####################################
        #工具栏
        toolbar=self.addToolBar('工具')
        toolbar.addAction(importImageAction)
        # toolbar.addAction(locationTableAction)
        # toolbar.addAction(resultPictureAction)


        #设置菜单禁用
        # self.startControlPointAction.setEnabled(False)
        self.endControlPointAction.setEnabled(False)


    def __setupStatusBar(self):
        self.statusBar().showMessage('准备就绪')



    def setExtraFileName(self):
        #设置工程文件名称
        self.projectFile=self.workDir+'/'+Attribute.projectName     
        #创建附加数据文件夹
        attributeDirName=self.workDir+'/'+Attribute.attributeDirName
        if not os.path.exists(attributeDirName):
            os.makedirs(attributeDirName)

    
    #设置工作目录
    def setWorkDir(self):
        MenuFuction.setWorkDir(self)
        self.statusBar().showMessage('准备就绪')

        
    #打开工程文件
    def openWork(self):
        MenuFuction.openWork(self)
        self.statusBar().showMessage('准备就绪')


    #导入图像
    def importImages(self):
        MenuFuction.importImages(self)
        self.statusBar().showMessage('准备就绪')
                       


    def importFullImages(self):
        MenuFuction.importFullImages(self)
        self.statusBar().showMessage('准备就绪')



    #查看图片坐标表格
    def lookLocationTable(self):
        if self.workDir is None:
            return
        latlonName=self.workDir+'/'+Attribute.imageLocationName
        table=LocationTable(self.projectFile,latlonName)



    #开始进行拼接
    def stitchingStart(self):
        MenuFuction.stitchingStart(self)



    #查看拼接结果
    def lookStitchingResult(self):
        if self.workDir is None:
            return
        resultImageName=self.workDir+'/'+Attribute.resultImageName
        self.centerUi.showFullImage(resultImageName)


    
    #导入控制点文件
    def importControlPoint(self):
        MenuFuction.importControlPoint(self)


    
    #显示/隐藏控制点
    def showControlPoint(self):
        if self.projectFile is None:
            return   
        if type(self.centerUi.imageViewer) is not ImageViewPoint:
            return

        if self.cpKernel is None:
            imageInfoFile=self.workDir+'/'+Attribute.segmentImageInfoFile

            self.cpKernel=KernelPoints(imageInfoFile)
            self.cpKernel.setWidget(self.centerUi.pointsViewer,self.centerUi.imageViewer)
            self.cpKernel.setImagePath(self.centerUi.currentImageFile)
            
        self.cpKernel.showPoints()

    
    #开始选择控制点
    def startControlPoint(self):
        if self.projectFile is None:
            return

        if type(self.centerUi.imageViewer) is not ImageViewPoint:
            return

        if self.cpKernel is  None:
            imageInfoFile=self.workDir+'/'+Attribute.segmentImageInfoFile

            self.cpKernel=KernelPoints(imageInfoFile)
            self.cpKernel.setWidget(self.centerUi.pointsViewer,self.centerUi.imageViewer)
            self.cpKernel.setImagePath(self.centerUi.currentImageFile)
        
        self.cpKernel.startSelect()
        #设置菜单启用与禁用
        self.startControlPointAction.setEnabled(False)
        self.endControlPointAction.setEnabled(True)


    #结束选择控制点
    def endControlPoint(self):
        
        if self.cpKernel is None:
            return

        self.cpKernel.endSelect()   

        #设置菜单启用与禁用
        self.startControlPointAction.setEnabled(True)
        self.endControlPointAction.setEnabled(False)


    #导入DXF
    def importDxf(self):
        if self.workDir is None:
            return
        dxfInfoName=self.workDir+'/'+Attribute.dxfInfoName
        ImportDxfDiaolg(dxfInfoName)



    #查看导入DXF结果表格
    def lookDxfTable(self):
        if self.workDir is None:
            return
        dxfInfoName=self.workDir+'/'+Attribute.dxfInfoName
        dxf=DxfTable(dxfInfoName)        


    def showDxfCircle(self):
        MenuFuction.showDxfCircle(self)




    #图像分割界面
    def segmentObject(self):
        MenuFuction.segmentObject(self)


    def lookReport(self):
        MenuFuction.lookReport(self)


    def showSegment(self):
        MenuFuction.showSegment(self)





    #相机表达参数
    def caliParmsOpen(self):
        command=self.programPath+'/modules/caliParms.exe'
        subprocess.Popen(command, startupinfo=Attribute.startupinfo, shell=False, close_fds=True)



    #图片畸变矫正
    def undistortOpen(self):
        command=self.programPath+'/modules/undistort.exe'
        subprocess.Popen(command,startupinfo=Attribute.startupinfo, shell=False, close_fds=True)


    def closeEvent(self, event):
        if self.cpKernel is not None:
            self.cpKernel.endSelect()



    def aboutInfo(self):
        QMessageBox.about(self,'关于','Hust, All rights reserved')











