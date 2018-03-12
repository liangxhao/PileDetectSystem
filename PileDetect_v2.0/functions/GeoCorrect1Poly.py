import numpy as np

class GeoCorrect1Poly(object):
    def __init__(self):
        self.A=None
        self.B=None


    def setData(self, imagePoints, geoPoints):
        assert len(geoPoints) == len(imagePoints)
        geoPoints = np.array(geoPoints)
        imagePoints = np.array(imagePoints)

        self.__compute(geoPoints,imagePoints)

        ###############计算误差
        imageError=imagePoints-np.matrix(self.groundsToPixels(geoPoints))
        imageError=np.sqrt(np.square(imageError[:,0])+np.square(imageError[:,1]))

        geoError=geoPoints-np.matrix(self.pixelsToGrounds(imagePoints))
        geoError=np.sqrt(np.square(geoError[:,0])+np.square(geoError[:,1]))

        imageError = imageError.round(3).T.tolist()[0]
        geoError = geoError.round(3).T.tolist()[0]

        return imageError,geoError



    def __compute(self,geoPoints,imagePoints):
        N = geoPoints.shape[0]
        X = geoPoints[:, 0]
        Y = geoPoints[:, 1]

        M = np.zeros([N, 3])
        M[:, 0] = 1
        M[:, 1] = X
        M[:, 2] = Y

        xImg = imagePoints[:, 0]
        yImg = imagePoints[:, 1]

        M = np.matrix(M)
        C1 = np.matrix(xImg).T
        C2 = np.matrix(yImg).T

        p = (M.T * M).I * M.T

        self.A = p * C1
        self.B = p * C2

        self.A = self.A.T.tolist()[0]
        self.B = self.B.T.tolist()[0]



    def getParms(self):
        return self.A+self.B


    def setParms(self,parms):
        assert len(parms)==6
        self.A=parms[0:3]
        self.B=parms[3:6]


    def groundsToPixels(self, points):
        points=np.array(points)
        X=points[:,0]
        Y=points[:,1]

        a0=self.A[0]
        a1=self.A[1]
        a2=self.A[2]

        b0 = self.B[0]
        b1 = self.B[1]
        b2 = self.B[2]

        xImg=a0+a1*X+a2*Y
        yImg=b0+b1*X+b2*Y

        return np.c_[xImg,yImg].tolist()


    def pixelsToGrounds(self,points):
        N=len(points)

        a0 = self.A[0]
        a1 = self.A[1]
        a2 = self.A[2]

        b0 = self.B[0]
        b1 = self.B[1]
        b2 = self.B[2]

        Y=np.matrix(points)
        M=np.matrix([[a1,a2],[b1,b2]]).T
        C=np.matrix([[a0,b0]]).repeat(N,0)

        geo=(Y-C)*M.I

        geo=geo.tolist()

        return geo



if __name__=='__main__':
    # pixels = [[1222015.55214358, 164008.019084179],
    #           [1231615.55214358, 164008.019084179],
    #           [1205215.55214358, 161858.019084179],
    #           [1236415.55214358, 161358.019084179],
    #           [1212415.55214358, 157058.019084179],
    #           [1205215.55214358, 149858.019084179],
    #           [1236415.55214358, 149358.019084179],
    #           [1214815.55214358, 149608.019084179],
    #           [1233415.55214358, 144458.019084029],
    #           [1219015.55214358, 144458.019084029]]
    # grounds = [[4664.56, 1260.72],
    #            [6507.59, 1295.05],
    #            [1287.71, 1533.59],
    #            [7502.16, 1851.34],
    #            [2663.33, 2538.96],
    #            [1138.07, 3869.29],
    #            [7407.7, 4221.74],
    #            [3023.11, 4051.09],
    #            [6759.76, 5134.66],
    #            [3868.71, 5060.51]]

    grounds = (np.random.random([4, 2]) * 100).tolist()
    pixels = (np.random.random([4, 2]) * 100).tolist()
    print(grounds)
    print(pixels)


    dd=GeoCorrect1Poly()
    e=dd.setData(pixels,grounds)

    print(dd.getParms())

    # p1=dd.geoToPixel(grounds)
    # p2=dd.pixelToGeo(pixels)
    # print(p1)
    # print(p2)