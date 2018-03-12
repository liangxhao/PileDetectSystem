from AffineTransform import AffineTransform
class TransformCoords(object):
    def __init__(self):
        self.__initVariable()


    def __initVariable(self):
        self.aff=None
        self.parms=None

    def setParmsFile(self,file):
        self.parmsFile=file


    def setPointsToSlove(self,pixel,ground):
        self.aff=AffineTransform(pixel,ground)
        self.parms=self.aff.getParms()


    def pixelToGround(self,pixel):
        return self.aff.pixelToGround(pixel)


    def groundToPixel(self,ground):
        return self.aff.groundToPixel(ground)


    #保存参数至文件
    def writeParms(self):
        with open(self.parmsFile,'w') as f:
            f.write("x=k1*x'+k2*y'+x0  k1,k2,x0:\n")
            f.write(str(self.parms[0][0])+','+str(self.parms[0][1])+','+str(self.parms[0][2])+'\n')
            f.write("y=k3*x'+k4*y'+y0  k3,k4,y0:\n")
            f.write(str(self.parms[1][0])+','+str(self.parms[1][1])+','+str(self.parms[1][2])+'\n')

    #从文件读取转换参数
    def readParms(self):
        self.parms=[]
        with open(self.parmsFile,'r') as f:
            next(f)
            # line=f.readline()#第一行

            line=f.readline().replace('\n','')#第二行
            item=[float(i) for i in line.split(',')]
            self.parms.append(item)

            next(f)
            # line=f.readline()#第三行

            line=f.readline().replace('\n','')#第四行
            item=[float(i) for i in line.split(',')]
            self.parms.append(item)

    #坐标变换的长度缩放比例（像素->地面）
    def __getScale(self):
        return (self.parms[0][0]**2+self.parms[0][1]**2)**(1/2)


    #计算长度的变化
    #1：像素->地面
    #2：地面->像素
    def lengthScale(self,length,flag=1):
        scale=self.__getScale()

        if flag==1:
            if type(length)==list:
                return [item*scale for item in length]           
            else:
                return length*scale
                
        else:
            if type(length)==list:
                return [item/scale for item in length]           
            else:
                return length/scale
