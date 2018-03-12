from functions.CoordsTransform import CoordsTransform
from functions.GeoCorrect1Poly import GeoCorrect1Poly
from functions.GeoCorrect2Poly import GeoCorrect2Poly


class PointsManager(object):
    def __init__(self, mode = 0):
        self.mode = mode
        self.__initVariable()


    def __initVariable(self):
        self.imageID=-1
        self.database=None

        # 是否修改点
        self.hasChange=False
        # 是否计算了转换参数
        self.hasParms=False

        # 0表示坐标转换模式
        # 1表示1次几何校正（仿射变换）
        # 2表示2次几何校正
        if self.mode == 0:
            self.needPoints = 3
            self.correctTool=CoordsTransform()
        elif self.mode == 1:
            self.needPoints = 4
            self.correctTool=GeoCorrect1Poly()
        elif self.mode == 2:
            self.needPoints = 7
            self.correctTool = GeoCorrect2Poly()



    #设置图片控件、点列表、图像列表控件，方便使用
    def setWidget(self,viewer,selPointer,fileViewer):
        self.viewer=viewer
        self.selPointer=selPointer
        self.fileViewer=fileViewer

        self.selPointer.addPoint.connect(self.__addPoint)
        self.selPointer.selectPoint.connect(self.__selectPoint)
        self.selPointer.deletePoint.connect(self.__deletePoint)
        self.viewer.pointSelectCoord.connect(self.__updateCoord)

    #设置数据库，方便查询
    def setDataBase(self,db):
        self.database=db
    
    #设置当前处理图片的id
    def setCurrentImageID(self,idx):
        self.imageID=idx
        self.__initShow()



    def __initShow(self):
        #地面控制点数据，显示
        ids,names,points=self.database.getCPoints()
        self.selPointer.setGroundPoints(ids,names,points)
        #已选择像素点数据，显示
        ids,names,points,pErrors,gErrors=self.database.getPixelCPoints(self.imageID)
        self.selPointer.setPixelPoints(ids,names,points,pErrors,gErrors)
        #图像上，显示
        self.viewer.setPoints(ids,names,points)

    #从Table中接收到要新增一个点的信息，发送给ImageView
    def __addPoint(self,idx,name):
        self.viewer.addPoint(idx,name)
        self.selPointer.setErrors(None,None)
        self.__updateImageState(0)

        self.hasChange=True



    #从Table上选择当前正在编辑的点的id，发送给ImageView
    def __selectPoint(self,idx):
        self.viewer.setCurrentPoint(idx)


    #从Table上删除某点的id，发送给ImageView
    def __deletePoint(self,idx):
        self.viewer.deletePoint(idx)
        self.__updateErrors()

        self.hasChange = True



    #图像上的点击更新了坐标值，传递给Table
    def __updateCoord(self,x,y):
        self.selPointer.setCurrentCoords(x,y)
        self.__updateErrors()



    def __updateErrors(self):
        if len(self.selPointer.idPs)<self.needPoints or [None,None] in self.selPointer.pPoints:
            self.selPointer.setErrors(None,None)
            self.__updateImageState(0)
            self.hasParms = False
            self.hasChange = True
            return
        
        pixels=self.selPointer.pPoints
        grounds=[]
        for idx in self.selPointer.idPs:
            pos=self.selPointer.idGs.index(idx)
            grounds.append(self.selPointer.gPoints[pos])

        pErrors,gErrors=self.correctTool.setData(pixels,grounds)

        self.selPointer.setErrors(pErrors,gErrors)

        #更新状态
        self.__updateImageState(1)
        self.hasParms = True
        self.hasChange = True


    #更新状态，并保存状态新增至数据库
    def __updateImageState(self,state):
        self.fileViewer.updateState(self.imageID,state)
        self.database.updateImageState(self.imageID,state_point=state)



    def __del__(self):
        if not self.hasChange:
            return

        # 数据库在此之前已经被关闭
        try:
            # 保存点
            self.database.insertSelectCPoints(self.imageID, self.selPointer.idPs,self.selPointer.pPoints, self.selPointer.pErrors, self.selPointer.gErrors)


            if not self.hasParms:
                return

            # 更新坐标转换参数模式
            self.database.updateImageParmsMode(self.imageID,self.mode)

            # 保存参数
            if self.mode == 0:
                parms = self.correctTool.getParms()
                self.database.updateTransformParms(self.imageID, parms)

            else:
                # 先保存坐标变换的参数
                pixels = self.selPointer.pPoints
                grounds = []
                for idx in self.selPointer.idPs:
                    pos = self.selPointer.idGs.index(idx)
                    grounds.append(self.selPointer.gPoints[pos])

                cf=CoordsTransform()
                cf.setData(pixels, grounds)
                cf_parms = cf.getParms()
                self.database.updateTransformParms(self.imageID, cf_parms)
                # 1阶几何校正模式
                if self.mode == 1:
                    poly1_parms = self.correctTool.getParms()
                    self.database.updateGeo1PolyParms(self.imageID, poly1_parms)
                # 2阶几何校正模式
                elif self.mode == 2:
                    A,B,Ainv,Binv = self.correctTool.getParms()
                    self.database.updateGeo2PolyParms(self.imageID, A,B,Ainv,Binv)

        except:
            print('直接退出，没有保存数据')



        
        


