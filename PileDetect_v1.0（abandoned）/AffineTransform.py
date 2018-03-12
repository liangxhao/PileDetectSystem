import numpy as np

class AffineTransform(object):
    def __init__(self,pixel=None,ground=None):
        #图像像素点 [[x,y]]
        # self.pixel=pixel
        #地面点 [[x,y]]
        # self.ground=ground

        
        self.parms=None

        if pixel is not None and ground is not None:
            self.__slove(pixel,ground)


    def __verification(self,pixel,ground):
        #最少控制点个数
        minPointsNum=3

        if len(ground)!=len(pixel):
            return False

        if len(ground)<minPointsNum:
            return False

        #去掉pixel中的空值       
        pixel=[p for p in pixel if p[0]!='']

        if len(pixel)<minPointsNum:
            return False
        
        return True



    #公式
    # 这里使用仿射变换
    # k* |cos(t) -sin(t)| 记为  |k1 k2|   而不是  |k1 -k2|
    #    |sin(t)  cos(t)|       |k3 k4|          |k2  k1|
    # (x,y)地面点；(x',y')像素点
    # x=k1*x'+k2*y'+x0
    # y=k3*x'+k4*y'+y0
    def __slove(self,pixel,ground):
        if not self.__verification(pixel,ground):
            return

        ground=[g for g,p in zip(ground,pixel) if p[0]!='']
        pixel=[p for p in pixel if p[0]!='']

        #最小二乘 Y=A*b
        A=np.matrix(pixel)
        Extend=np.ones(A.shape[0]).T
        A=np.c_[A,Extend]

        Y=np.matrix(ground)
        Y1=Y[:,0]
        Y2=Y[:,1]

        invA=(A.T*A).I*A.T

        P1=invA*Y1
        P2=invA*Y2

        #[k1 k3
        # k2 k4
        # x0 y0]
        self.parms=np.c_[P1,P2]

    def setParms(self,parms):
        self.parms=np.matrix(parms).T

    def getParms(self):
        #[[k1,k2,x0]
        # [k3,k4,y0]]

        return self.parms.T.tolist()

    def pixelToGround(self,points):
        if len(points)==0:
            return []
            
        #points每行是一个像素点 [x,y]
        A=np.matrix(points)
        Extend=np.ones(A.shape[0]).T
        A=np.c_[A,Extend]
        result=A*self.parms

        result=result.round(5)
        
        return result.tolist()


    def groundToPixel(self,points):
        if len(points)==0:
            return []
        #points每行是一个地面点 [x,y]
        ground=np.matrix(points)
        #(x',y')=(x,y)*[k1 k3
        #               k2 k4]+(x0,y0)

        xy0=self.parms[2,:].repeat(ground.shape[0],0)
        k=self.parms[0:2,0:2]

        result=(ground-xy0)*k.I

        result=result.round(3)
        
        return result.tolist()


if __name__=='__main__':
    

    pixels=[[1222015.55214358,164008.019084179],
            [1231615.55214358,164008.019084179],
            [1205215.55214358,161858.019084179],
            [1236415.55214358,161358.019084179],
            [1212415.55214358,157058.019084179],
            [1205215.55214358,149858.019084179],
            [1236415.55214358,149358.019084179],
            [1214815.55214358,149608.019084179],
            [1233415.55214358,144458.019084029],
            [1219015.55214358,144458.019084029]]
    grounds=[[4664.56,1260.72],
            [6507.59,1295.05],
            [1287.71,1533.59],
            [7502.16,1851.34],
            [2663.33,2538.96],
            [1138.07,3869.29],
            [7407.7,4221.74],
            [3023.11,4051.09],
            [6759.76,5134.66],
            [3868.71,5060.51]]
    aff=AffineTransform(pixels,grounds)
    parms=aff.getParms()

    # print(parms)
    # 
    p=aff.pixelToGround(pixels)
    # p=aff.groundToPixel(grounds)
    print(p)


