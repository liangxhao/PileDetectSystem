import os
class ControlPointsFile(object):

    def __init__(self,file):
        self.__pointFileName=file
        self.__initVariable()


    def __initVariable(self):
        self.groundPoints=[]
        self.groundPointsNum=0

        self.pixelPoints=[]
        self.pixelPointsNum=0

        self.pointsName=[]


    def getPoints(self):
        self.__readFile()
        return self.groundPoints,self.pixelPoints,self.pointsName


    def savePixelPoints(self,pixelPoints):
        self.pixelPoints=pixelPoints
        self.pixelPointsNum=len([item for item in pixelPoints if item[0]!=''])

        self.__savePixelFile()
 


    def __readFile(self):
        if not os.path.exists(self.__pointFileName):
            return
        
        self.pointsName.clear()
        self.groundPoints.clear()
        self.pixelPoints.clear()

        with open(self.__pointFileName,'r') as f:
            #读取地面控制点
            line=f.readline().replace('\n','')
            self.groundPointsNum=int(line)#地面控制点个数

            for i in range(self.groundPointsNum):
                point=[0,0]

                line=f.readline().replace('\n','')
                item=line.split(',')

                name=str(item[0])
                point[0]=float(item[1])
                point[1]=float(item[2])

                # point=[float(i) for i in line.split(',')]

                self.groundPoints.append(point)
                self.pointsName.append(name)
           
            #读取图像控制点
            self.pixelPoints=[['','']]*self.groundPointsNum
            line=f.readline().replace('\n','')
            self.pixelPointsNum=int(line)#图像已选择控制点个数

            for i in range(self.pixelPointsNum):
                line=f.readline().replace('\n','')
                (index,x,y)=line.split(',')
                self.pixelPoints[int(index)]=[float(x),float(y)]

        
    def __savePixelFile(self):
        #先读取地面控制点部分
        with open(self.__pointFileName,'r') as f:
            contens=f.readlines()
            if len(contens)<2:
                return

            num=int(contens[0].replace('\n',''))
            contens=contens[0:num+1]
        #像素控制点部分重写
        with open(self.__pointFileName,'w') as f:
            f.writelines(contens)
            #写图像控制点
            f.write(str(self.pixelPointsNum)+'\n')
            
            for index,item in enumerate(self.pixelPoints):
                if item[0]=='':
                    continue
                f.write(str(index)+','+str(item[0])+','+str(item[1])+'\n')
