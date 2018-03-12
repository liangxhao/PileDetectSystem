import os
class TfwReader(object):
    def __init__(self):
        super(TfwReader,self).__init__()
        self.limit=None
        self.size=None


    #设置图像的尺寸
    def setImageSize(self,size):
        self.size=size


    def setTfwFile(self,tfw):
        #文件不存在
        if not os.path.isfile(tfw):
            return False

        try:
            with open(tfw,'r') as f:
                contents=f.readlines()
                #x方向的像素分辨率
                A=float(contents[0])
                #y方向的像素分辨率
                E=float(contents[3])
                #左上角中心x坐标
                C=float(contents[4])
                #左上角中心y坐标
                F=float(contents[5])

            #计算图像范围
            x_min=C
            y_max=F

            x_max=A*self.size[0]+x_min
            y_min=y_max-E*self.size[1]

            self.limit=[x_min,x_max,y_min,y_max]

            return True

        except:
            return False