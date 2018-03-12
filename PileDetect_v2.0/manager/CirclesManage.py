from functions.CoordsTransform import CoordsTransform
from functions.GeoCorrect2Poly import GeoCorrect2Poly
from functions.GeoCorrect1Poly import GeoCorrect1Poly
from dialogs.SegmentCircleDialog import SegmentCircleDialog
from PyQt5.QtWidgets import QMessageBox



class CirclesManager(object):
    def __init__(self, geoCorrectMode):
        self.geoCorrectMode=geoCorrectMode
        self.__initVariable()

    def __initVariable(self):
        self.imageID=-1
        self.database=None
        self.state=0

        self.allIds=[]
        self.allCircle=[]
        self.hasSegment=[]

        self.__segmentDialog=None


    def setsegmentProgram(self,segmentProgram):
        self.segmentProgram=segmentProgram


    # 设置图片控件、点列表、图像列表控件，方便使用
    def setWidget(self,viewer,selCircler,fileViewer):
        self.viewer=viewer
        self.selCircler=selCircler
        self.fileViewer=fileViewer


        self.selCircler.stateUpdated.connect(self.__updateImageState)
        self.selCircler.selectPoint.connect(self.__selectPoint)
        self.selCircler.deletePoint.connect(self.__deletePoint)
        self.selCircler.segmentCall.connect(self.__segmentCircle)
        self.viewer.circleSelectCoord.connect(self.__updateCoord)

    #设置数据库，方便查询
    def setDataBase(self,db):
        self.database=db


    #设置当前处理图片的id
    def setCurrentImageID(self,idx,parent):
        self.imageID=idx
        state_point,self.state=self.database.getImageState(idx)
        if self.state+state_point==0:
            QMessageBox.information(parent,'提示','请先选择控制点')
            return

        self.__initShow()

        # 用来转换半径值
        self.radiusTools=CoordsTransform()
        r_parms = self.database.getTransformParms(idx)
        self.radiusTools.setParms(r_parms)

        # 用来转换坐标值
        if self.geoCorrectMode==0:
            self.coordsTools = CoordsTransform()
            parms = self.database.getTransformParms(idx)

        elif self.geoCorrectMode==1:
            self.coordsTools = GeoCorrect1Poly()
            parms = self.database.getGeo1PloyParms(idx)

        elif self.geoCorrectMode==2:
            self.coordsTools = GeoCorrect2Poly()
            parms=self.database.getGeo2PloyParms(idx)

        self.coordsTools.setParms(parms)



    def __initShow(self):
        ids,names,clc_Points,clc_radiuses,sel_Points,sel_radiuses, gErrors=self.database.getCirclesByImage(self.imageID)
        self.allIds=ids
        # 记录所有基桩
        self.allCircle=dict(zip(ids,zip(clc_Points,clc_radiuses)))

        # 所有备选点（圆），显示
        self.selCircler.setPoints(ids,names,gErrors)
        self.selCircler.setState(self.state)

        self.viewer.setAlternativeCircles(ids,names,clc_Points,clc_radiuses)

        idSels=[]
        pointSels=[]
        radisuSels=[]
        for idx,point,gr,ge in zip(ids,sel_Points,sel_radiuses, gErrors):
            if ge is not None:
                idSels.append(idx)
                pointSels.append(point)
                radisuSels.append(gr)
        # 记录下已经分割过的圆的id
        self.hasSegment=idSels
        # 图像上，所有选择过的点，显示（已经分割过的圆）
        self.viewer.setSelectedCircles(idSels,pointSels,radisuSels)



    #从Table上选择当前正在编辑的点的id，发送给ImageView
    def __selectPoint(self,idx):
        self.viewer.setCurrentPoint(idx)


    #从Table上删除某点的id，发送给ImageView
    def __deletePoint(self,idx):
        self.viewer.deleteCircle(idx)
        self.__saveCoordsToDB(idx,[None,None],None,[None,None],None,None)
        # 从记录中移除该点
        if idx in self.hasSegment:
            self.hasSegment.remove(idx)



    #图像上的点击更新了坐标值，传递给Table
    def __updateCoord(self,px,py,pr):
        pPoint=[px,py]
        gPoint=self.coordsTools.pixelsToGrounds([pPoint])[0]
        gr = self.radiusTools.lengthPixelsToGrounds(pr)
        #当前圆的id
        idx=self.viewer.currentID
        gx,gy,_=self.database.getCircleById(idx)

        error=[gx-gPoint[0],gy-gPoint[1]]
        gError=(error[0]**2+error[1]**2)**0.5

        self.__updateErrors(idx,gError)

        self.__saveCoordsToDB(idx,pPoint,pr,gPoint,gr,gError)

        if idx not in self.hasSegment:
            self.hasSegment.append(idx)

        #计算pixels误差
        # gPoint=[gx,gy]
        # pPoint=self.cf.groundsToPixels([gPoint])[0]

        # error=[px-pPoint[0],py-pPoint[1]]
        # pError=(error[0]**2+error[1]**2)**0.5
        # print(idx,',',pError)


    #显示计算出的误差
    def __updateErrors(self,idx,gError):
        self.selCircler.setPoint(idx,gError)

    
    #保存选择的数据至数据库
    def __saveCoordsToDB(self,idx,pPoint,pr,gPoint,gr,gError):
        self.database.updateSelectCircleByImage(self.imageID,idx,pPoint,pr,gPoint,gr,gError)




    #更新状态，并保存状态新增至数据库
    def __updateImageState(self,state):
        if state==1:
            self.fileViewer.updateState(self.imageID,2)
            self.database.updateImageState(self.imageID,state_circle=1)
        else:
            self.fileViewer.updateState(self.imageID,1)
            self.database.updateImageState(self.imageID,state_circle=0)
    

    # 响应图像分割的按钮
    def __segmentCircle(self):
        self.selCircler.clearSelect()

        # idx_noSegment=set(self.allCircle.keys())-set(self.hasSegment)
        idx_noSegment=[idx for idx in self.allIds if idx not in self.hasSegment]
        name=self.database.getImageNameById(self.imageID)

        if self.__segmentDialog is not None:
            del self.__segmentDialog
            self.__segmentDialog=None

        self.__segmentDialog=SegmentCircleDialog()
        self.__segmentDialog.setSegmentInfo(name,self.allCircle,idx_noSegment,self.segmentProgram)

        self.__segmentDialog.segmentResultSignal.connect(self.__receiveSegementResult)
        self.__segmentDialog.endSegmentSignal.connect(self.__endSegment)

        self.__segmentDialog.exec_()
        self.__segmentDialog.setModal(True)



    # 接收自动分割的结果
    def __receiveSegementResult(self,idx,px,py,pr):
        self.__selectPoint(idx)
        self.selCircler.selectCircle(idx)
        self.__updateCoord(px,py,pr)
        self.viewer.setSingleSelectedCircle(idx,px,py,pr)


    # 结束分割
    def __endSegment(self):
        self.selCircler.clearSelect()
        self.viewer.clearSelect()





        



        
        


