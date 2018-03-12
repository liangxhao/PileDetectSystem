import subprocess
import os
from PyQt5.QtWidgets import QFileDialog,QFileDialog,QMessageBox

from Attribute import Attribute
from ImportPoints import ImportPoints
from StitchingDialog import StitchingDialog
from SegmentDialog import SegmentDialog
from ProjectFile import ProjectFile
from CircleObjFile import CircleObjFile
from ReportFile import ReportFile
from SegmentImageInfo import SegmentImageInfo


class MenuFuction(object):

    #导入图像
    @classmethod
    def importImages(cls,parent):
        if parent.workDir is None:
            return

        files,ext = QFileDialog.getOpenFileNames(parent,  
                                    "导入待拼接图像",  
                                    './',  
                                    "Image Files (*.jpg *tif)")  
        if len(files)>0:
            parent.centerUi.fileViewer.insertFiles(files)
            #保存工程文件
            pj=ProjectFile(parent.projectFile)     
            pj.setNoramlImages(files)     

            latlonName=parent.workDir+'/'+Attribute.imageLocationName

            #从图片中获取坐标信息        
            command=[]
            command.append(parent.programPath+'/modules/exifcoords.exe')
            command.append(parent.projectFile)
            command.append(latlonName)
            
            subprocess.Popen(command, startupinfo=Attribute.startupinfo, shell=False, close_fds=True)


    #导入全图
    @classmethod
    def importFullImages(cls,parent):
        if parent.workDir is None:
            return

        files,ext = QFileDialog.getOpenFileNames(parent,  
                                    "导入全图",  
                                    './',  
                                    "Image Files (*.jpg *tif)")  
        if len(files)>0:
            parent.centerUi.fullViewer.insertFiles(files) 
            #保存工程文件
            pj=ProjectFile(parent.projectFile)     
            pj.setFullImages(files) 
   



    #打开工程文件
    @classmethod
    def openWork(cls,parent):
        file, ext = QFileDialog.getOpenFileName(parent,  
                                    "文件选择",  
                                    './',  
                                    "PRO Files (*.pro)")          
        if len(file)>0:
            parent.projectFile=file
            parent.workDir=os.path.split(file)[0]
            parent.setExtraFileName()

            #保存工程文件
            pj=ProjectFile(parent.projectFile)     
            (files1,files2)=pj.getImages() 
        
            parent.centerUi.fileViewer.insertFiles(files1)#显示文件名列表
            parent.centerUi.fullViewer.insertFiles(files2)#显示文件名列表

            controlPointName=parent.workDir+'/'+Attribute.controlPointName
            parent.centerUi.pointsViewer.setPointsFile(controlPointName)


    #设置工作目录
    @classmethod
    def setWorkDir(cls,parent):
        parent.workDir=QFileDialog.getExistingDirectory()
        if len(parent.workDir)>0:
            parent.setExtraFileName()

            #保存工程文件
            pj=ProjectFile(parent.projectFile)     
            pj.createProject() 

        else:
            parent.workDir=None



    #导入控制点文件
    @classmethod
    def importControlPoint(cls,parent):
        if parent.workDir is None:
            return
        # if parent.cpKernel is not None:
        #     if self.cpKernel.isEditPoints:
        #         return
        
        file, ext = QFileDialog.getOpenFileName(parent,  
                                    "文件选择",  
                                    './',  
                                    "Data Files (*.xlsx *.xls *.txt)")          
        if len(file)>0:
            controlPointName=parent.workDir+'/'+Attribute.controlPointName

            pointsFile=ImportPoints(file,controlPointName)
            num=len(pointsFile.points)
            if num>0:
                QMessageBox.information(parent,'成功','共导入'+str(num)+'个点')
                parent.centerUi.pointsViewer.setPointsFile(controlPointName)

        
    #开始进行拼接
    @classmethod
    def stitchingStart(cls,parent):
        if parent.workDir is None:
            return
        else:
            latlonName=parent.workDir+'/'+Attribute.imageLocationName
            resultImageName=parent.workDir+'/'+Attribute.resultImageName

            command=[]
            command.append(parent.programPath+'/modules/stitching.exe')
            command.append(parent.projectFile)
            command.append(resultImageName)
            command.append(latlonName)

            stitching=StitchingDialog(command)


    #显示导入的cad圆
    @classmethod
    def showDxfCircle(cls,parent):
        if parent.workDir is None:
            return
        objFile=parent.workDir+'/'+Attribute.dxfInfoName

        cof=CircleObjFile(objFile)
        (names,location,radius)=cof.getObjCircleFromGround()
        parent.centerUi.showCircleByGround(names,location,radius)


    #图像分割界面
    @classmethod
    def segmentObject(cls,parent):
        if parent.workDir is None:
            return

        #获取图片文件
        imageInfoFile=parent.workDir+'/'+Attribute.segmentImageInfoFile
        info=SegmentImageInfo(imageInfoFile)
        imagePath=info.getImageFilePath()#需要分割的图片文件
        if len(imagePath)==0:
            return

        parmsFile=parent.workDir+'/'+Attribute.pixelToGroundParms#参数文件
        segmentInputFile=parent.workDir+'/'+Attribute.segmentInputFile#用于分割的输入文件
        segmentOutputFile=parent.workDir+'/'+Attribute.segmentOutputFile#分割结果文件
        dxfInfoName=parent.workDir+'/'+Attribute.dxfInfoName#保存dxf导入的圆形目标的文件
        controlPointName=parent.workDir+'/'+Attribute.controlPointName#控制点文件
        reportExportFile=parent.workDir+'/'+Attribute.reportExportFile#报告文件
        

        command=[]
        
        command.append(controlPointName)
        command.append(parmsFile)
        command.append(dxfInfoName)
        command.append(imagePath)
        command.append(segmentInputFile)
        command.append(segmentOutputFile)
        command.append(reportExportFile)
        command.append(parent.programPath+'/modules/segment.exe')

        segment=SegmentDialog(command,parent.centerUi.imageViewer)


    #导出报告文件
    @classmethod
    def lookReport(cls,parent):
        if parent.workDir is None:
            return

        reportExportFile=parent.workDir+'/'+Attribute.reportExportFile#报告文件

        if not os.path.exists(reportExportFile):
            return

        file, ext = QFileDialog.getSaveFileName(parent,  
                                    "文件保存",  
                                    './',  
                                    "Excel Files (*.xls)")          
        if len(file)>0:
            report=ReportFile(reportExportFile)

            report.exportTable(file)

            QMessageBox.information(parent,'成功','导出成功！')



    @classmethod
    def showSegment(cls,parent):
        if parent.workDir is None:
            return
        #获取图片文件
        imageInfoFile=parent.workDir+'/'+Attribute.segmentImageInfoFile
        info=SegmentImageInfo(imageInfoFile)
        imagePath=info.getImageFilePath()#需要分割的图片文件
        if len(imagePath)==0:
            return

        parent.centerUi.showSegmentImage(imagePath)

        #读取已经分割结果文件并显示
        segmentOutputFile=parent.workDir+'/'+Attribute.segmentOutputFile#分割结果文件
        circleObjName=parent.workDir+'/'+Attribute.dxfInfoName#保存dxf导入的圆形目标的文件

        if not os.path.exists(segmentOutputFile):
            return

        if not os.path.exists(circleObjName):
            return

        fileManager=CircleObjFile(circleObjName)
        (names,_,_)=fileManager.getObjCircleFromGround()
        if len(names)==0:
            return

        (index,segXYPixel,segRPixel,_)=fileManager.readSegmentOutput(segmentOutputFile)
        if len(index)==0:
            return

        parent.centerUi.imageViewer.setNames(names)
        parent.centerUi.imageViewer.showData(index,segXYPixel,segRPixel)


        

