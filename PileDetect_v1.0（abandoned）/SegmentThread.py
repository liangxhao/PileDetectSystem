from PyQt5.QtCore import QThread, pyqtSignal, Qt
from ControlPointsFile import ControlPointsFile
from TransformCoords import TransformCoords
from CircleObjFile import CircleObjFile
from Attribute import Attribute
from ReportFile import ReportFile

import codecs
import locale
import os
from subprocess import Popen
import subprocess

#求解坐标转换参数
#转换圆心实际坐标为像素坐标，并保存

class SegmentThread(QThread):
    SignalResult = pyqtSignal(str)
    SignalNames = pyqtSignal(list)

    def __init__(self, command,*args, **kwargs):
        super(SegmentThread, self).__init__(*args, **kwargs)

        self.__pointsFile=command[0] #控制点文件
        self.__parmsFile=command[1] #参数文件
        self.__objFile=command[2] #圆心坐标文件
        self.__segmentImage=command[3]#分割的图片文件
        self.__segmentInputFile=command[4] #分割的临时输入文件
        self.__segmentOutputFile=command[5] #分割的输出结果

        self.__reportExportFile=command[6]#分割结果的统计报告

        self.__programExe=command[7]#segment.exe文件

        self.__initVariable()

    def __initVariable(self):
        self.fileManager=CircleObjFile(self.__objFile)

        self.stopCommand='taskkill /F /IM segment.exe'
        self.ExErunning = 1
        self.Pyruning=1

    def stopExe(self):
        self.ExErunning = 0
        if hasattr(self, "process"):
            self.process.kill()
            os.system(self.stopCommand)#调用外部命令结束


    def stop(self):
        self.Pyruning=0
        self.stopExe()
        



    def run(self):
        ################################控制点使用-开始
        #读取控制点数据
        cp=ControlPointsFile(self.__pointsFile)
        (groundPoints, pixelPoints, _)=cp.getPoints()

        #解算转换参数并保存
        tfc=TransformCoords()
        tfc.setParmsFile(self.__parmsFile)
        tfc.setPointsToSlove(pixelPoints,groundPoints)
        tfc.writeParms()
        ################################控制点使用-完毕


        ################################圆的坐标使用-开始
        #读取圆的实际坐标-从dxf导出来的那个坐标
        (names,oriXYGround,oriRGround)=self.fileManager.getObjCircleFromGround()

        self.SignalNames.emit(names)

        #转变圆心坐标及其半径 实际->像素
        oriXYPixel=tfc.groundToPixel(oriXYGround)
        oriRPixel=tfc.lengthScale(oriRGround,2)
        
        #保存转换至像素的结果
        self.fileManager.setObjCircleByPixel(names,oriXYPixel,oriRPixel)

        #讲数据文件转换为exe读取的文件格式（主要是取整，修改标号）
        self.fileManager.convertDataToSegement(self.__segmentInputFile)

        #图像分割的参数命令
        cmd=[]
        cmd.append(self.__programExe)
        cmd.append(self.__segmentImage)
        cmd.append(self.__segmentInputFile)
        cmd.append(self.__segmentOutputFile)

        #开始进行图像分割
        self.process = Popen(cmd, startupinfo=Attribute.startupinfo,stdout=subprocess.PIPE, shell=False)

        while self.ExErunning:
            data = self.process.stdout.readline()
            if not data:
                # self.SignalExit.emit()
                break
            else:
                data = data.decode(
                    codecs.lookup(locale.getpreferredencoding()).name)
                self.SignalResult.emit(data)

        if self.Pyruning == 0:
            return

        #读取分割结果，像素
        #序号，圆心坐标，半径，是否分割成功
        (index,segXYPixel,segRPixel,segFlag)=self.fileManager.readSegmentOutput(self.__segmentOutputFile)

        #将像素坐标转换为实际地面坐标
        segXYGround=tfc.pixelToGround(segXYPixel)
        segRGround=tfc.lengthScale(segRPixel)

        self.fileManager.saveSegmentResultByGround(self.__segmentOutputFile,index,segXYGround,segRGround,segFlag)
        ################################圆的坐标使用-结束

        ################################生成报告
        report=ReportFile(self.__reportExportFile)

        report.generateReportByData(names,oriXYGround,oriRGround,index,segXYGround,segRGround,segFlag)
        
        self.SignalResult.emit("OK")


    






