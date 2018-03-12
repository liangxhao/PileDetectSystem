import numpy as np
class CoordsTransform(object):
    def __init__(self):
        self.__initVariable()

    def __initVariable(self):
        #[kcos(theta), ksin(theta), x0, y0]
        self.parms=None

    #    [x -y 1 0      [m1      [x'
    #     y  x 0 1]  *   m2    =  y']
    #                    x0
    #                    y0]
    # pixels ---->grounds
    def setData(self,pixels,grounds, needError=True):
        pixels=np.matrix(pixels)
        pixels[:,1]=-pixels[:,1]#左手坐标系转右手坐标系，y轴翻转
        grounds=np.matrix(grounds)


        num=pixels.shape[0]#点个数
        pos1=range(0,2*num,2)
        pos2=range(1,2*num,2)


        X=self.__rearrangePixel(pixels)
        Y=grounds.flatten().T

        self.parms=(X.T*X).I*X.T*Y

        if not needError:
            return

        ###############计算误差
        gErrors=X*self.parms-Y
        gErrors=np.sqrt(np.square(gErrors[pos1])+np.square(gErrors[pos2]))
        gErrors=gErrors.round(3).T.tolist()[0]


        R=np.matrix([[self.parms[0,0],-self.parms[1,0]],[self.parms[1,0],self.parms[0,0]]])
        D=np.matrix([self.parms[2,0],self.parms[3,0]]).T
        D=np.tile(D,(1,num))

        Y=np.reshape(Y,[num,2]).T

        pErrors=R.I*(Y-D)-pixels.T
        pErrors=np.sqrt(np.square(pErrors[0,:])+np.square(pErrors[1,:]))
        pErrors=pErrors.round(3).tolist()[0]

        return pErrors,gErrors


    def __rearrangePixel(self,pixels):
        A=pixels.copy()
        A[:,1]=-A[:,1]

        B=pixels.copy()
        B[:,[0,1]]=B[:,[1,0]]

        num=A.shape[0]#点个数

        X=np.mat(np.zeros([2*num,2]))

        pos1=range(0,2*num,2)
        X[pos1,:]=A
        pos2=range(1,2*num,2)
        X[pos2,:]=B

        eye=np.eye(2);

        E=np.tile(eye,(num,1))
        X=np.c_[X,E]

        return X


    #[m1,m2,x0,y0]
    def getParms(self):
        return self.parms.T.tolist()[0]

    #设置参数
    def setParms(self,parms):
        self.parms=np.matrix(parms).T


    def pixelsToGrounds(self,pixels):
        num=len(pixels)
        if num==0:
            return []

        pixels=np.matrix(pixels)
        pixels[:,1]=-pixels[:,1]#左手坐标系转右手坐标系，y轴翻转


        X=self.__rearrangePixel(pixels)
        Y=X*self.parms

        Y=np.reshape(Y,[num,2])

        return Y.tolist()


    def groundsToPixels(self,grounds):
        num=len(grounds)
        if num==0:
            return []

        R=np.matrix([[self.parms[0,0],-self.parms[1,0]],[self.parms[1,0],self.parms[0,0]]])
        D=np.matrix([self.parms[2,0],self.parms[3,0]]).T
        D=np.tile(D,(1,num))

        Y=np.matrix(grounds).T

        X=(R.I*(Y-D)).T 
        X[:,1]=-X[:,1]
        
        return X.tolist()

    def __getScale(self):
        k=(self.parms[0,0]**2+self.parms[1,0]**2)**0.5
        return k.tolist()


    def lengthGroundsToPixels(self,length):
        k = self.__getScale()
        if type(length) is not list:
            return length/k
        else:
            return [x/k for x in length]

    def lengthPixelsToGrounds(self,length):
        k = self.__getScale()

        if type(length) is not list:
            return length*k
        else:
            return [x*k for x in length]





        




        
if __name__=='__main__':
    aff=CoordsTransform()

    # pixels=[[1222015.55214358,164008.019084179],
    #         [1231615.55214358,164008.019084179],
    #         [1205215.55214358,161858.019084179],
    #         [1236415.55214358,161358.019084179],
    #         [1212415.55214358,157058.019084179],
    #         [1205215.55214358,149858.019084179],
    #         [1236415.55214358,149358.019084179],
    #         [1214815.55214358,149608.019084179],
    #         [1233415.55214358,144458.019084029],
    #         [1219015.55214358,144458.019084029]]
    # grounds=[[4664.56,1260.72],
    #         [6507.59,1295.05],
    #         [1287.71,1533.59],
    #         [7502.16,1851.34],
    #         [2663.33,2538.96],
    #         [1138.07,3869.29],
    #         [7407.7,4221.74],
    #         [3023.11,4051.09],
    #         [6759.76,5134.66],
    #         [3868.71,5060.51]]

    pixels=[[10,-15],[20,-40],[30,-55]]
    grounds=[[-21.9615,84.6410],[-88.5641,169.2820],[-120.5256,233.9230]]

    pe,ge=aff.setData(pixels,grounds)
    print(pe)
    print(ge)
    parms=aff.getParms()

    groundsss=aff.pixelsToGrounds(pixels)
    pixelsss=aff.groundsToPixels(grounds)

    print(groundsss)        
    print(pixelsss)
