from access.DataBase import DataBase
from functions.CirclesAssign import CirclesAssign
from PyQt5.QtCore import Qt,QThread,pyqtSignal 
from tempfile import NamedTemporaryFile
from access.TfwReader import TfwReader
from access.DxfWriter import DxfWriter

import codecs
import locale
import subprocess

import numpy as np 
import os

class Composite(QThread):
    processStep=pyqtSignal(int)

    def __init__(self,dbFile):
        super(Composite,self).__init__()
        self.dbFile=dbFile 
        self.__initVariable()


    def __initVariable(self):
        #相机的标定文件
        # self.cameraFile=None
        #图像叠加后的图片名称
        self.compositeImage=None
        #exe文件
        self.programFile=None

        #图片数量
        self.numImages=None

        #图像尺寸
        self.imageSize=None

        #输入到exe，描述图片参数的文件
        with NamedTemporaryFile('w+t', delete=False) as f:
            self.imageDescFile=f.name.replace("\\",'/')
            

    #设置exe文件
    def setProgram(self,file):
        self.programFile=file

    # 设置地理校正模式
    def setGeoCorrectMode(self,mode):
        self.geoCorrectMode=mode


    #设置用于图像信息
    def setCompositeInfo(self,imageName):
        self.compositeImage=imageName


        self.outDxfFile=self.compositeImage[:-4]+'.dxf'
        self.tfwFile=self.compositeImage[:-4]+'.tfw'



    def __remove(self):
        try:
            if os.path.isfile(self.compositeImage):
                os.remove(self.compositeImage)

            if os.path.isfile(self.outDxfFile):
                os.remove(self.outDxfFile)

            if os.path.isfile(self.tfwFile):
                os.remove(self.tfwFile)
        except:
            pass



    #更新图像的四个点坐标
    def __updateImageLimits(self):
        assign=CirclesAssign(self.dbFile,self.geoCorrectMode)
        assign.updateLimit()

        del assign

    #获取可以参与拼接的图像
    def getImageDesc(self):
        #计算图像四个角点的坐标
        self.__updateImageLimits()

        #从数据看中获取图片名、角度参数、坐标等信息
        database=DataBase(self.dbFile)
        database.connectDB()
        images,parms,limit_x,limit_y=database.getImageDesc()
        database.closeDB()

        del database

        self.numImages=len(images)
        if self.numImages==0:
            return

        #根据角度参数计算每张图的旋转角度
        angles=self._clcAngles(parms)

        #写入文本文件
        self.outputToTxt(images,angles,limit_x,limit_y)

        

    #计算角度
    def _clcAngles(self,parms):
        parms=np.array(parms)

        angles=np.arctan(parms[:,1]/parms[:,0])

        index=parms[:,0]<0
        angles[index]=angles[index]+np.pi

        index=(parms[:,0]>0) & (parms[:,0]>0)
        angles[index]=angles[index]+np.pi*2

        return angles.tolist()


    
    #将所需数据写入txt
    def outputToTxt(self,images,angles,limit_x,limit_y):
        file=self.imageDescFile
        num=len(images)

        with open(file,'w') as f:
            f.write(str(num)+'\n')

            for i in range(num):
                f.write(images[i]+'\n')

            for i in range(num):
                f.write(str(angles[i])+'\n')

            for i in range(num):
                s=str(limit_x[i][0])+','+str(limit_x[i][1])+','+str(limit_x[i][2])+','+str(limit_x[i][3])+'\n'
                f.write(s)

            for i in range(num):
                s=str(limit_y[i][0])+','+str(limit_y[i][1])+','+str(limit_y[i][2])+','+str(limit_y[i][3])+'\n'
                f.write(s)


    def __getLimit(self):
        tfw=TfwReader()
        tfw.setImageSize(self.imageSize)

        if not tfw.setTfwFile(self.tfwFile):
            return False

        self.limit=tfw.limit

        return True


    def run(self):
        #删除已存在的文件
        self.__remove()

         #子线程的cmd窗口属性
        startupinfo = subprocess.STARTUPINFO()
        startupinfo.dwFlags |= subprocess.STARTF_USESHOWWINDOW
        startupinfo.wShowWindow = subprocess.SW_HIDE


        self.getImageDesc()
        
        self.processStep.emit(self.numImages)

        #如果没有可用图像，就停止
        if self.numImages==0:
            return

        #执行的命令
        cmd=[self.programFile,self.imageDescFile,self.compositeImage]
        # if self.cameraFile is not None:
        #     cmd.append(self.cameraFile)

        #
        self.process = subprocess.Popen(cmd, startupinfo=startupinfo,stdout=subprocess.PIPE, shell=False)

        idx=0
        size=None

        while True:
            data = self.process.stdout.readline()
            if not data:
                # self.SignalExit.emit()
                break
            else:
                data = data.decode(
                    codecs.lookup(locale.getpreferredencoding()).name).replace('\r\n','')
                
                if '#' not in data:
                    idx=int(data)
                else:
                    data=data[1:].split(',')
                    self.imageSize=(int(data[0]),int(data[1]))

                self.processStep.emit(idx)



        if self.__getLimit():
            

            dxf=DxfWriter()
            dxf.setImageInfo(self.compositeImage,self.imageSize,self.limit)
            dxf.setData(self.dbFile,self.outDxfFile)
            

        self.processStep.emit(idx+1)


    