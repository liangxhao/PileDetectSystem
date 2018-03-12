from access.DataBase import DataBase
from functions.CoordsTransform import CoordsTransform
from functions.GeoCorrect1Poly import GeoCorrect1Poly
from functions.GeoCorrect2Poly import GeoCorrect2Poly
from PIL import Image
from PyQt5.QtCore import QObject,pyqtSignal
import numpy as np

class CirclesAssign(QObject):
    processImage=pyqtSignal(int)

    def __init__(self,dbFile, geoCorrectMode):
        super(CirclesAssign,self).__init__()
        self.geoCorrectMode=geoCorrectMode
        self.database=DataBase(dbFile)
        self.database.connectDB()
        self.__initVariable()

    def __del__(self):
        self.database.closeDB()


    def __initVariable(self):
        self.width=None
        self.height=None
        # self.radius=None

        #下面的，长度相同
        self.ids=[]#图像id号
        self.parms=[]#图像转换参数
        self.limits=[]#图像范围


    def run(self):
        if not self.updateLimit():
            return False

        self.__getCircles()
        return True

    #更新图像的四个点坐标范围
    def updateLimit(self):
        if not self.__getImage():
            return False

        self.__convertLimits()
        return True



    def __getImage(self):
        self.ids,images,self.parms=self.database.getImageTransformParmsByState()
        num=len(self.ids)

        #发送图像个数
        self.processImage.emit(num)

        if num==0:
            return False

        #获取图像尺寸
        img=Image.open(images[0])
        self.width,self.height=img.size
        del img

        return True


    #计算每张图的地理坐标范围，并保存至数据库
    def __convertLimits(self):
        cf=CoordsTransform()
        pixels=[[0,0],[0,self.height-1],[self.width-1,self.height-1],[self.width-1,0]]

        #计算
        for idx,parm in zip(self.ids,self.parms):     
            cf.setParms(parm)
            limit=cf.pixelsToGrounds(pixels)
            self.limits.append(limit)
        #保存
        self.database.updateImageLimit(self.ids,self.limits)



    #读取圆信息并开始处理
    def __getCircles(self):
        idCs,_,locations,radius=self.database.getCircles()
        if len(idCs)==0:
            self.processImage.emit(len(self.limits)-1)
            return
        # self.radius=radius[0]

        idCs=np.matrix(idCs)
        locations=np.matrix(locations)

        ####
        self.__filter(idCs,locations,radius)


    #根据地理坐标范围过滤，并求转换坐标
    def __filter(self,idCs,points,radius):
        cf=CoordsTransform()


        #对每张图
        for i,limit in enumerate(self.limits):
            #过滤的结果
            index=self.__pointsInRect(limit,points)
            #没有圆落在该图像内
            if index[0].size==0:
                self.processImage.emit(i)
                continue
            #位于某图像上的圆的id
            idCs_Image=idCs.T[index].tolist()[0]

            #位于某图像上的圆的地理坐标
            grounds_Image=points[index[0]].tolist()

            #计算对应的像素坐标
            cf.setParms(self.parms[i])

            #计算像素半径
            radiusPixel=cf.lengthGroundsToPixels(radius[i])

            if self.geoCorrectMode==0:
                pixels = cf.groundsToPixels(grounds_Image)
            else:
                if self.geoCorrectMode == 1:
                    coordsTool = GeoCorrect1Poly()
                    polyParms = self.database.getGeo1PloyParms(self.ids[i])
                elif self.geoCorrectMode == 2:
                    coordsTool = GeoCorrect2Poly()
                    polyParms=self.database.getGeo2PloyParms(self.ids[i])

                coordsTool.setParms(polyParms)
                pixels=coordsTool.groundsToPixels(grounds_Image)

            self.__saveCirclesInImageToDB(self.ids[i],idCs_Image,pixels,radiusPixel)

            #发送当前正在处理的图像序号
            self.processImage.emit(i)

    

    #保存至数据库
    def __saveCirclesInImageToDB(self,image_id,circle_ids,pixels,radius):
        self.database.insertSelectCircle(image_id,circle_ids,pixels,radius)




    #圆是否在矩形中，返回在矩形中的序号（行号）
    def __pointsInRect(self,limit,points):
        num=points.shape[0]

        limit=np.matrix(limit)
        #矩形框逆时针四个点
        p1=limit[0]
        p2=limit[1]
        p3=limit[2]
        p4=limit[3]

        #向量
        p2p0=points-p2
        p1p2=p2-p1
        p3p0=points-p3
        p3p4=p4-p3

        p1p0=points-p1
        p1p4=p4-p1
        p2p0=points-p2
        p2p3=p3-p2

        #(x1,y1)x(x2,y2)=x1*y2-x2*y1
        #p2p0   x p1p2
        s1=p2p0[:,0]*p1p2[0,1]-p1p2[0,0]*p2p0[:,1]
        #p3p0   X p3p4
        s2=p3p0[:,0]*p3p4[0,1]-p3p4[0,0]*p3p0[:,1]
        #p1p0   x p1p4
        s3=p1p0[:,0]*p1p4[0,1]-p1p4[0,0]*p1p0[:,1]
        #p2p0   x p2p3
        s4=p2p0[:,0]*p2p3[0,1]-p2p3[0,0]*p2p0[:,1]


        d1=np.multiply(s1,s2)>0
        d2=np.multiply(s3,s4)<0

        flag=d1 & d2

        return np.where(flag==True)















if __name__=='__main__':
    dbFile='C:/Users/liang/Desktop/tmp/acc.pro'
    ca=CirclesAssign(dbFile)
    ca.run()



        







        

        

    