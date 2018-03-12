from PyQt5.QtWidgets import QMainWindow,QAction,qApp
from PyQt5.QtGui import QIcon
from abc import ABCMeta, abstractmethod
from CenterUI import CenterUI
import sys,os

import icon 

class MainUI(QMainWindow):
    __metaclass__ = ABCMeta

    def __init__(self,parent=None):
        super(MainUI,self).__init__()
        self.__initUI()
        self.__initVariable()


    def __initUI(self):
        self.__createMenu()
        self.centerUi=CenterUI()          
        self.setCentralWidget(self.centerUi)
        self.showMaximized()      
        self.setWindowTitle('无人机摄影测量检测基桩偏位系统')
        self.setWindowIcon(QIcon(':/image/program.png'))

        


    def __initVariable(self):
        #获取工作目录
        if getattr(sys, 'frozen', False):
            self.programPath = os.path.dirname(sys.executable)
        elif __file__:
            self.programPath = os.path.dirname(__file__)
        
        self.programPath=self.programPath.replace("\\",'/')#主程序所在目录


    def __createMenu(self):
        ###############################################文件
        newProjectAction=QAction(QIcon(':image/newProject.png'),'&新建工程   ',self)
        newProjectAction.triggered.connect(self.newProject)
        openProjectAction=QAction(QIcon(':image/openProject.png'),'&打开工程',self)
        openProjectAction.triggered.connect(self.openProject)
        importImageAction=QAction(QIcon(':image/importImage.png'),'&导入图像   ',self)
        importImageAction.triggered.connect(self.importImages)
        setParmsModeAction=QAction('&设置校正模式',self)
        setParmsModeAction.triggered.connect(self.setParmsMode)
        exitAction=QAction('&退出 ',self)
        exitAction.triggered.connect(qApp.quit)

        #############################################数据
        importControlPointAction=QAction('&导入控制点数据',self)
        importControlPointAction.triggered.connect(self.importControlPoint)
        lookControlPointAction=QAction('&查看控制点数据',self)
        lookControlPointAction.triggered.connect(self.lookControlPoint)

        importCADAction=QAction('&导入CAD数据  ',self)
        importCADAction.triggered.connect(self.importCAD)
        lookCircleAction=QAction('&查看CAD数据  ',self)
        lookCircleAction.triggered.connect(self.lookCircle)

        ###############################################选点
        selectCPointsAction=QAction('&选点模式  ',self)
        selectCPointsAction.triggered.connect(self.selectCPoints)
        exitCPointsAction=QAction('&退出选点模式  ',self)
        exitCPointsAction.triggered.connect(self.exitCPoints)


        ###############################################选基桩
        estimateCirclesAction=QAction('&基桩坐标转换  ',self)
        estimateCirclesAction.triggered.connect(self.estimateCircles)
        selectCirclesAction=QAction('&标注基桩模式  ',self)
        selectCirclesAction.triggered.connect(self.selectCircles)
        exitCirclesAction=QAction('&退出标注基桩模式  ',self)
        exitCirclesAction.triggered.connect(self.exitCircles)

        ###############################################报告
        collectAction=QAction('&汇总统计  ',self)
        collectAction.triggered.connect(self.collect)
        lookReportAction=QAction('&查看统计表  ',self)
        lookReportAction.triggered.connect(self.lookReport)
        exportReportAction=QAction('&导出报告  ',self)
        exportReportAction.triggered.connect(self.exportReport)
        exportCompositeAction=QAction('&导出拼接图像  ',self)
        exportCompositeAction.triggered.connect(self.exportComposite)
        lookCompositeAction=QAction('&显示拼接图像  ',self)
        lookCompositeAction.triggered.connect(self.lookComposite)


        ##################
        caliAction=QAction('  &相机标定  ',self)
        caliAction.triggered.connect(self.caliParmsOpen)
        undistortAction=QAction('  &畸变校正  ',self)
        undistortAction.triggered.connect(self.undistortOpen)


        #菜单栏
        menubar=self.menuBar()

        #“文件”菜单
        fileMenu=menubar.addMenu('  &文件  ')
        fileMenu.addAction(newProjectAction)
        fileMenu.addAction(openProjectAction)
        fileMenu.addAction(importImageAction)
        fileMenu.addAction(setParmsModeAction)
        fileMenu.addAction(exitAction)

        #“数据”菜单
        dataMenu=menubar.addMenu('  &数据  ')
        dataMenu.addAction(importControlPointAction)
        dataMenu.addAction(lookControlPointAction)
        dataMenu.addAction(importCADAction)
        dataMenu.addAction(lookCircleAction)

        #”选点“菜单
        cpointsMenu=menubar.addMenu('  &控制点  ')
        cpointsMenu.addAction(selectCPointsAction)
        cpointsMenu.addAction(exitCPointsAction)

        #选圆
        circlesMenu=menubar.addMenu('  &基桩  ')
        circlesMenu.addAction(estimateCirclesAction)
        circlesMenu.addAction(selectCirclesAction)
        circlesMenu.addAction(exitCirclesAction)     
        
        #报告   
        reportMenu=menubar.addMenu('  &报告  ')
        reportMenu.addAction(collectAction)
        reportMenu.addAction(lookReportAction)
        reportMenu.addAction(exportReportAction)
        reportMenu.addAction(exportCompositeAction)
        reportMenu.addAction(lookCompositeAction)


        #第六项菜单
        caliMenu=menubar.addMenu('   &标定   ')
        caliMenu.addAction(caliAction)
        caliMenu.addAction(undistortAction)

        
        #工具栏
        toolbar=self.addToolBar('工具')
        toolbar.addAction(newProjectAction)
        toolbar.addAction(openProjectAction)
        toolbar.addAction(importImageAction)


    #新建工程
    @abstractmethod
    def newProject(self):pass


    #新建工程
    @abstractmethod
    def importImages(self):pass


    #打开工程
    @abstractmethod
    def openProject(self):pass


    # 设置校正模式
    @abstractmethod
    def setParmsMode(self):pass


    #导入控制点
    @abstractmethod
    def importControlPoint(self):pass

    #查看控制点
    @abstractmethod
    def lookControlPoint(self):pass

    #导入CAD
    @abstractmethod
    def importCAD(self):pass


    #查看导入的CAD数据
    @abstractmethod
    def lookCircle(self):pass


    #选点模式
    @abstractmethod
    def selectCPoints(self):pass


    #退出选点模式
    @abstractmethod
    def exitCPoints(self):pass

    #计算每张图像上的圆
    @abstractmethod
    def estimateCircles(self):pass


    #选圆模式
    @abstractmethod
    def selectCircles(self):pass

    #退出选圆模式
    @abstractmethod
    def exitCircles(self):pass

    #统计结果
    @abstractmethod
    def collect(self):pass

    #查看报告
    @abstractmethod
    def lookReport(self):pass

    #导出报告
    @abstractmethod
    def exportReport(self):pass


     #导出图像
    @abstractmethod
    def exportComposite(self):pass

    #查看拼接后的图像（与圆叠加显示）
    @abstractmethod
    def lookComposite(self):pass


    #相机表达参数
    @abstractmethod
    def caliParmsOpen(self):pass
        

    #图片畸变矫正
    @abstractmethod
    def undistortOpen(self):pass
        
