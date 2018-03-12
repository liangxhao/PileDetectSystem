import numpy as np


class GeoCorrect2Poly(object):
    def __init__(self):
        self.A = None
        self.B = None

        self.Ainv=None
        self.Binv=None

    def setData(self, imagePoints: list, geoPoints: list):
        assert len(geoPoints) == len(imagePoints)
        geoPoints = np.array(geoPoints)
        imagePoints = np.array(imagePoints)

        self.__compute(geoPoints, imagePoints)

        ###############计算误差
        imageError = imagePoints - np.matrix(self.groundsToPixels(geoPoints))
        imageError = np.sqrt(np.square(imageError[:, 0]) + np.square(imageError[:, 1]))

        geoError = geoPoints - np.matrix(self.pixelsToGrounds(imagePoints))
        geoError = np.sqrt(np.square(geoError[:, 0]) + np.square(geoError[:, 1]))

        imageError = imageError.round(3).T.tolist()[0]
        geoError = geoError.round(3).T.tolist()[0]

        ##
        # gg=self.pixelsToGrounds([[2945.86, 1691.083]])

        return imageError, geoError

    def __compute(self, geoPoints, imagePoints):
        self.A, self.B=self._computeDetail(geoPoints, imagePoints)
        self.Ainv, self.Binv=self._computeDetail(imagePoints, geoPoints)


    def _computeDetail(self, Mat1, Mat2):
        N = Mat1.shape[0]
        X = Mat1[:, 0]
        Y = Mat1[:, 1]

        M = np.zeros([N, 6])
        M[:, 0] = 1
        M[:, 1] = X
        M[:, 2] = Y
        M[:, 3] = X * Y
        M[:, 4] = X * X
        M[:, 5] = Y * Y

        xImg = Mat2[:, 0]
        yImg = Mat2[:, 1]

        M = np.matrix(M)
        C1 = np.matrix(xImg).T
        C2 = np.matrix(yImg).T

        p = (M.T * M).I * M.T

        A = p * C1
        B = p * C2

        A = A.T.tolist()[0]
        B = B.T.tolist()[0]

        return A,B



    def getParms(self):
        return self.A, self.B, self.Ainv, self.Binv

    def setParms(self,parms):
        A, B, Ainv, Binv = parms
        assert len(A) == len(B) == 6
        assert len(Ainv) == len(Binv) == 6
        self.A = A
        self.B = B

        self.Ainv=Ainv
        self.Binv=Binv




    def groundsToPixels(self, points):
        points = np.array(points)
        X = points[:, 0]
        Y = points[:, 1]

        X2 = X * X
        XY = X * Y
        Y2 = Y * Y

        a00 = self.A[0]
        a10 = self.A[1]
        a01 = self.A[2]
        a11 = self.A[3]
        a20 = self.A[4]
        a02 = self.A[5]

        b00 = self.B[0]
        b10 = self.B[1]
        b01 = self.B[2]
        b11 = self.B[3]
        b20 = self.B[4]
        b02 = self.B[5]

        xImg = a00 + a10 * X + a01 * Y + a20 * X2 + a11 * XY + a02 * Y2
        yImg = b00 + b10 * X + b01 * Y + b20 * X2 + b11 * XY + b02 * Y2

        return np.c_[xImg, yImg].tolist()

    def pixelsToGrounds(self, points):
        points = np.array(points)
        X = points[:, 0]
        Y = points[:, 1]

        X2 = X * X
        XY = X * Y
        Y2 = Y * Y

        a00 = self.Ainv[0]
        a10 = self.Ainv[1]
        a01 = self.Ainv[2]
        a11 = self.Ainv[3]
        a20 = self.Ainv[4]
        a02 = self.Ainv[5]

        b00 = self.Binv[0]
        b10 = self.Binv[1]
        b01 = self.Binv[2]
        b11 = self.Binv[3]
        b20 = self.Binv[4]
        b02 = self.Binv[5]

        xImg = a00 + a10 * X + a01 * Y + a20 * X2 + a11 * XY + a02 * Y2
        yImg = b00 + b10 * X + b01 * Y + b20 * X2 + b11 * XY + b02 * Y2

        return np.c_[xImg, yImg].tolist()



if __name__=='__main__':
    dd=GeoCorrect2Poly()
    np.random.seed(1)
    p1=(np.random.random([6,2])*100).tolist()
    p2=(np.random.random([6,2])*100).tolist()

    print(p2)
    # print(p2)

    ss=dd.setData(p1,p2)

    # pp2=dd.geoToPixel(p1)
    # print(p2)
    # print(pp2)

    pp1=dd.pixelsToGrounds(p2)
    pp2=dd.groundsToPixels(p1)

    # print(dd.A)
    # print(dd.B)
    # print(dd.getCoeff())

    print(pp2)
    print(ss)
    parms=dd.getParms()
    dd2=GeoCorrect2Poly()
    dd2.setParms(parms)
    pp22=dd2.groundsToPixels(p1)
    print(pp22)












