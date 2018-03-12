import os
class CircleObjFile(object):
    def __init__(self,file=None):
        self.__objFile=file

    #保存(dxf导出的坐标)
    def setObjCircleByGround(self,names,location,radius):
        with open(self.__objFile,'w') as f:
            f.write(str(len(location))+'\n')

            for (name,loc,rad) in zip(names, location, radius):
                f.write(name+','+str(loc[0])+','+str(loc[1])+','+str(rad)+'\n')


    #读取-实际坐标(dxf导出的坐标)
    def getObjCircleFromGround(self):
        names=[]
        location=[]
        radius=[]
        try:
            with open(self.__objFile,'r') as f:
                line=f.readline().replace('\n','')
                num=int(line)
                for i in range(num):
                    line=f.readline()
                    item=line.replace('\n','').split(',')

                    names.append(item[0])
                    location.append([float(item[1]),float(item[2])])
                    radius.append(float(item[3]))
        except:
            names.clear()
            location.clear()
            radius.clear()

            
        return names,location,radius

    #保存像素坐标(坐标转换后的结果）
    def setObjCircleByPixel(self,names,location,radius):
        
        try:
            with open(self.__objFile,'r') as f:
                contens=f.readlines()

                num=int(contens[0].replace('\n',''))
                contens=contens[0:num+1]
        except:
            contens=['0\n']


        with open(self.__objFile,'w') as f:
            f.writelines(contens)

            f.write(str(len(names))+'\n')

            for (name,loc,rad) in zip(names,location,radius):
                f.write(name+','+str(loc[0])+','+str(loc[1])+','+str(rad)+'\n')


    #将坐标转换后的结果（像素）整理一下格式，保存到另一个文件中，作为分割程序exe的输入
    def convertDataToSegement(self,tmpFile):
        location=[]
        radius=[]
        try:
            with open(self.__objFile,'r') as f:
                line=f.readline().replace('\n','')
                num=int(line)
                for i in range(num):
                    next(f)
                next(f)

                for i in range(num):
                    line=f.readline()
                    item=line.replace('\n','').split(',')

                    location.append([round(float(item[1])),round(float(item[2]))])#转换成整数
                    radius.append(round(float(item[3])))

            with open(tmpFile,'w') as f:
                f.write(str(num)+'\n')

                for i in range(num):
                    f.write(str(i)+','+str(location[i][0])+','+str(location[i][1])+','+str(radius[i])+'\n')

        except:
            return

    #读取exe输出的文件（像素）
    def readSegmentOutput(self,segmentOutFile):
        index=[]
        segXY=[]
        segR=[]
        segFlag=[]

        try:
            with open(segmentOutFile,'r') as f:
                #第一行文字说明
                line=f.readline()
                line=f.readline().replace('\n','')
                num=int(line)

                for i in range(num):
                    line=f.readline()
                    item=line.replace('\n','').split(',')

                    index.append(int(item[0]))
                    segXY.append([int(item[1]),int(item[2])])
                    segR.append(int(item[3]))
                    segFlag.append(int(item[4]))

        except:
            index.clear()
            segXY.clear()
            segR.clear()
            segFlag.clear()

        return index,segXY,segR,segFlag






    #转换exe输出的文件,像素转换为地面坐标，保存
    def saveSegmentResultByGround(self,segmentOutFile,index,points,radius,flag):

        if not os.path.exists(segmentOutFile):
            return
            
        #保证只运行一次吧
        with open(segmentOutFile,'r+') as f:
            next(f)
            num=len(index)
            f.write(str(num)+'\n')
            for i in range(num):
                f.write(str(index[i])+','+str(points[i][0])+','+str(points[i][1])+','+str(radius[i])+','+str(flag[i])+'\n')

    

if __name__=='__main__':
    ccc=CircleObjFile()
    segmentOutFile='C:/users/liangxhao/Desktop/project/attribute/segmentOutput.dat'
    ccc.readSegmentOutput(segmentOutFile)