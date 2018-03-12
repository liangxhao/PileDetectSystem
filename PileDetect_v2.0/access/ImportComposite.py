from .DataBase import DataBase
from functions.CoordsTransform import CoordsTransform
from access.TfwReader import TfwReader
from PyQt5.QtGui import QPixmap,QImage
from PIL import Image,ImageQt
import os
import numpy as np

class ImportComposite(object):
    def __init__(self):
        self.__initVariable()

    def __initVariable(self):
        #图像的地理坐标范围
        #x_min,x_max,y_min,y_max
        self.limit=None
        #图像尺寸
        self.size=None
        #图像
        self.image=None


    def loadImage(self,file):
        if not self.__readImage(file):
            return False

        #读取文件错误
        if not self.__readTFW(file):
            return False

        return True


    def loadData(self,dbFile):
        database=DataBase(dbFile)
        database.connectDB()
        names,points,points_r,gPoints,gPoints_r,_=database.getReport(needNull=False)
        # radius=database.getAnyRadius()

        database.closeDB()
        del database

        if len(names)==0:
            return [],[],[],None

        #四个角点的像素点坐标
        pixels=[[0,0],
                [0,self.size[1]],
                [self.size[0],self.size[1]],
                [self.size[0],0]]
        #四个角点的地面坐标
        grounds=[[self.limit[0],self.limit[3]],
                 [self.limit[0],self.limit[2]],
                 [self.limit[1],self.limit[2]],
                 [self.limit[1],self.limit[3]]]

        cf=CoordsTransform()
        cf.setData(pixels,grounds,False)

        points=cf.groundsToPixels(points)
        points_r=cf.lengthGroundsToPixels(points_r)
        gPoints=cf.groundsToPixels(gPoints)
        gPoints_r=cf.lengthGroundsToPixels(gPoints_r)

        # 找到落在图像之外的去除
        if len(points)!=0:
            names = np.array(names)
            points = np.array(points)
            points_r=np.array(points_r)
            d1 = (points[:, 0] > 0) & (points[:, 0] < self.size[0]) & (points[:, 1] > 0) & (points[:, 1] < self.size[1])
            index1 = np.where(d1 == True)
            names = names[index1].tolist()
            points = points[index1].tolist()
            points_r=points_r[index1].tolist()

        if len(gPoints)!=0:
            gPoints=np.array(gPoints)
            gPoints_r=np.array(gPoints_r)
            d2=(gPoints[:,0]>0) & (gPoints[:,0]<self.size[0]) & (gPoints[:,1]>0) & (gPoints[:,1]<self.size[1])
            index2=np.where(d2==True)
            gPoints=gPoints[index2].tolist()
            gPoints_r = gPoints_r[index2].tolist()


        # radius=cf.lengthGroundsToPixels(radius)

        return names,points,points_r,gPoints,gPoints_r


        


    def __readImage(self,file):
        # frame=Image.open(file)  
        # self.image=ImageQt.ImageQt(frame).convertToFormat(QImage.Format_RGB888)
        try:
            frame=Image.open(file)
            self.image=ImageQt.ImageQt(frame).convertToFormat(QImage.Format_RGB888)
            # self.image=QPixmap(file)
            self.size=frame.size

            return True
        except:
            self.image=None
            return False


    #读取图像地理坐标信息
    def __readTFW(self,file):
        tfwFile=file[:-4]+'.tfw'

        tfw=TfwReader()
        tfw.setImageSize(self.size)
        if not tfw.setTfwFile(tfwFile):
            return False

        self.limit=tfw.limit

        return True
        




if __name__=='__main__':
    import sys
    from PyQt5.QtWidgets import QApplication
    app = QApplication(sys.argv)

    file='C:/Users/liang/Desktop/test/11.tif'
    dbFile='C:/Users/liang/Desktop/tmp/acc.pro'
    imp=ImportComposite()
    imp.loadImage(file)
    imp.loadData(dbFile)

    print(imp.size)