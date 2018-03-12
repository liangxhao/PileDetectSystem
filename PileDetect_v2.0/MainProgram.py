from MainUI import MainUI
from access.ImportPoints import ImportPoints
from dialogs.ImportCADDialog import ImportCADDiaolg
from dialogs.LookCircleDialog import LookCircleDialog
from widgets.CircleTable import CircleTable
from widgets.CPointsTable import CPointsTable
from access.DataBase import DataBase
from manager.PointsManage import PointsManager
from manager.CirclesManage import CirclesManager
from dialogs.EstCirclesDialog import EstCirclesDialog
from dialogs.CollCirclesDialog import CollCirclesDialog
from widgets.ReportTable import ReportTable
from access.Report import Report
from dialogs.CompositeDialog import CompositeDialog
from dialogs.GeoCorrectDialog import GeoCorrectDialog
from manager.LookComposite import LookComposite

from PyQt5.QtWidgets import QFileDialog,QMessageBox
import os
import subprocess

class MainProgram(MainUI):
    def __init__(self,parent=None):
        super(MainProgram,self).__init__()
        self.__initVariable()



    def __initVariable(self):
        self.projectFile=None
        self.database=None

        self.cpManager=None
        self.cirManager=None

        self.title=''#窗口标题

        #当前模式
        #-1 尚未打开工程
        #0 一般模式
        #1 选择控制点模式
        #2 选择基桩模式
        self.currentMode=0
        self.tmpObject=None
        self.geoCorrectMode=0
    
    #检查当前状态
    def __checkMode(self,mode):
        if self.database is None:
            return False

        if self.currentMode != mode:
            return False

        return True


    #新建工程
    def newProject(self):
        if self.projectFile is not None:
            QMessageBox.information(self,'提示','请先关闭当前工程')
            return
        
        self.projectFile, ext=QFileDialog.getSaveFileName(self,
                                                    "新建工程文件",
                                                    '',
                                                    "Project File (*.pro)")
        if len(self.projectFile)>0:
            #创建数据库
            self.database=DataBase(self.projectFile)
            self.database.setSQLFile(self.programPath+'/config/database.sql')
            self.database.createDB()
            #连接数据库
            self.database.connectDB()

            self.title=os.path.split(self.projectFile)[1]#获取文件名
            self.title=self.windowTitle()+'——'+self.title
            self.setWindowTitle(self.title)

        else:
            self.projectFile=None

    #导入图像
    def importImages(self):
        if not self.__checkMode(0):
            return

        files,ext = QFileDialog.getOpenFileNames(self,  
                                    "导入图像",  
                                    '',  
                                    "Image Files (*.jpg *tif)")  
        if len(files)>0:
            self.database.insertImage(files)
            ids,files,states=self.database.getImages()
            self.centerUi.fileViewer.insertFiles(ids,files,states) 
            


    #打开工程
    def openProject(self):
        if self.projectFile is not None:
            QMessageBox.information(self,'提示','请先关闭当前工程')
            return

        
        self.projectFile, ext = QFileDialog.getOpenFileName(self,  
                                    "文件选择",  
                                    './',  
                                    "pro Files (*.pro)")          
        if len(self.projectFile)>0:
            #连接数据库
            self.database=DataBase(self.projectFile)
            self.database.connectDB()
            #查询图像
            ids,files,states=self.database.getImages() 

            self.centerUi.fileViewer.insertFiles(ids,files,states)#显示文件名列表
            self.geoCorrectMode=self.database.getCorrectMode()
            
            self.title=os.path.split(self.projectFile)[1]#获取文件名
            self.title=self.windowTitle()+'——'+self.title
            self.setWindowTitle(self.title)

        else:
            self.projectFile=None



    # 设置校正模式
    def setParmsMode(self):
        if not self.__checkMode(0):
            return

        dialog=GeoCorrectDialog(self.geoCorrectMode, self)
        lastMode=self.geoCorrectMode

        self.geoCorrectMode=dialog.getMode()

        if lastMode!=self.geoCorrectMode:
            self.database.setCorrectMode(self.geoCorrectMode)

            # 更新数据库所有数据
            self.database.resetImageState(self.geoCorrectMode)
            self.centerUi.fileViewer.resetState()




    #导入控制点
    def importControlPoint(self):
        if not self.__checkMode(0):
            return

        file, ext = QFileDialog.getOpenFileName(self,  
                                    "文件选择",  
                                    './',  
                                    "Data Files (*.xlsx *.xls *.txt)")          
        if len(file)>0:
            #导入
            pointsFile=ImportPoints(file)
            names,points=pointsFile.getPoints()
            #保持
            self.database.insertCPoints(names,points)
            num=len(names)
            if num>0:
                 QMessageBox.information(self,'成功','共导入'+str(num)+'个点')



    #查看控制点
    def lookControlPoint(self):
        if not self.__checkMode(0):
            return

        _,names,points=self.database.getCPoints()
        table=CPointsTable(self)
        table.setContents(names,points)

        table.exec_()


    #导入CAD
    def importCAD(self):
        if not self.__checkMode(0):
            return

        ImportCADDiaolg(self.database,self).exec_()


    #查看导入的CAD数据
    def lookCircle(self):
        if not self.__checkMode(0):
            return
        look=LookCircleDialog(self)
        look.exec_()


        if look.flag==1:   
            ids,names,locations,radius=self.database.getCircles()

            table=CircleTable(self)
            table.setDataBase(self.database)
            table.setContents(ids,names,locations,radius)
            table.exec_()

        elif look.flag==2:
            _,names,locations,radius=self.database.getCircles()
            self.centerUi.showImportCircle(names,locations,radius)


    #选点模式
    def selectCPoints(self):
        if not self.__checkMode(0):
            return
        self.currentMode=1
        ###
        self.centerUi.setMode(1)   
        self.centerUi.fileViewer.selectImage.connect(self.__selectCPointsManager)

        title=self.title+'（选点模式）'
        self.setWindowTitle(title)
        


    def __selectCPointsManager(self,image_idx,file):
        if not os.path.exists(file):
            QMessageBox.information(self,'失败','图像不存在，请检查路径')
            return

        name=os.path.basename(file)

        title=self.title+'——'+name+'（选点模式）'
        self.setWindowTitle(title)
        self.centerUi.addPointListView()

        self.cpManager=PointsManager(mode=self.geoCorrectMode)
        self.cpManager.setWidget(self.centerUi.imageViewer,self.centerUi.pLister,self.centerUi.fileViewer)
        self.cpManager.setDataBase(self.database)
        self.cpManager.setCurrentImageID(image_idx)
        


    #退出选点模式
    def exitCPoints(self):
        if not self.__checkMode(1):
            return

        self.currentMode=0

        # if self.cpManager is None:
        #     return
            
        self.centerUi.setMode(0)
        self.centerUi.deletePointListView()
        self.cpManager=None
        self.setWindowTitle(self.title)
        self.centerUi.fileViewer.selectImage.disconnect(self.__selectCPointsManager)
    

    #计算每张图像上的圆
    def estimateCircles(self):
        if not self.__checkMode(0):
            return

        estDialog=EstCirclesDialog(self.geoCorrectMode,self)
        estDialog.setDBFile(self.projectFile)
        estDialog.exec_()



    #选圆模式
    def selectCircles(self):
        if not self.__checkMode(0):
            return
        self.currentMode=2


        self.centerUi.setMode(2)   
        self.centerUi.fileViewer.selectImage.connect(self.__selectCirclesManager)

        title=self.title+'（标注基桩模式）'
        self.setWindowTitle(title)
        


    def __selectCirclesManager(self,image_idx,file):
        if not os.path.exists(file):
            QMessageBox.information(self,'失败','图像不存在，请检查路径')
            return

        name=os.path.basename(file)

        title=self.title+'——'+name+'（标注基桩模式）'
        programFile = self.programPath + '/modules/segment.exe'

        self.setWindowTitle(title)
        self.centerUi.addCircleListView()

        self.cirManager=CirclesManager(self.geoCorrectMode)
        self.cirManager.setsegmentProgram(programFile)
        self.cirManager.setWidget(self.centerUi.imageViewer,self.centerUi.cLister,self.centerUi.fileViewer)
        self.cirManager.setDataBase(self.database)
        self.cirManager.setCurrentImageID(image_idx,self)
        



    #退出选圆模式
    def exitCircles(self):
        if not self.__checkMode(2):
            return

        self.currentMode=0

        # if self.cirManager is None:
        #     return
            
        self.centerUi.setMode(0)
        self.centerUi.deleteCircleListView()
        self.cirManager=None
        self.setWindowTitle(self.title)
        self.centerUi.fileViewer.selectImage.disconnect(self.__selectCirclesManager)


    #统计结果
    def collect(self):
        if not self.__checkMode(0):
            return
        collDialog=CollCirclesDialog(self)
        collDialog.setDBFile(self.projectFile)
        collDialog.exec_()


    #查看报告
    def lookReport(self):
        if not self.__checkMode(0):
            return
        names,errors=self.database.getReportWithoutCoords()

        table=ReportTable(self)
        table.setContents(names,errors)
        table.exec_()


    #导出报告
    def exportReport(self):
        if not self.__checkMode(0):
            return

        names,points,point_r,gPoints,gPoints_r,errors=self.database.getReport()

        file, ext = QFileDialog.getSaveFileName(self,  
                                    "文件保存",  
                                    './',  
                                    "Excel Files (*.xls)")          
        if len(file)>0:
            report=Report(file)
            report.setContents(names,points,point_r,gPoints,gPoints_r,errors)
            QMessageBox.information(self,'成功','导出成功！')




    #导出图像
    def exportComposite(self):
        if not self.__checkMode(0):
            return

        programFile=self.programPath+'/modules/composite.exe'

        compDialog=CompositeDialog(self)
        compDialog.setParms(programFile,self.projectFile)

        compDialog.exec_()



    #查看拼接后的图像（与圆叠加显示）
    def lookComposite(self):
        if not self.__checkMode(0):
            return

        file, ext = QFileDialog.getOpenFileName(self,  
                                    "文件选择",  
                                    '',  
                                    "Image (*.tif)")          
        if len(file)>0:
            self.centerUi.showImportComposite()
            
            self.tmpObject=LookComposite(self)
            self.tmpObject.setImageViewer(self.centerUi.imageViewer)
            self.tmpObject.setFile(file,self.projectFile)


    #相机表达参数
    def caliParmsOpen(self):
        command=self.programPath+'/modules/calib.exe'
        # os.system(command)
        subprocess.Popen(command,  shell=False, close_fds=True)



    #图片畸变矫正
    def undistortOpen(self):
        command=self.programPath+'/modules/undist.exe'
        # os.system(command)
        subprocess.Popen(command,shell=False, close_fds=True)


    # def closeEvent(self,event):
    #     if self.database is not None:
    #         self.database.closeDB()
    #
    def __del__(self):
        if self.database is not None:
            self.database.closeDB()


        

  

             










