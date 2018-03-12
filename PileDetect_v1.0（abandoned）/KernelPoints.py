import os
from ImageView import ImageViewPoint
from SegmentImageInfo import SegmentImageInfo
class KernelPoints(object):
    def __init__(self,imageInfoFile):
        super().__init__()
        self.imageInfoFile=imageInfoFile
        self.__initVariable()


    def __initVariable(self):

        #当前正在选择的控制点编号
        self.currentID=-1
        #当前是否显示了点
        self.showFlag=False
        #当前是否正在选点
        self.isEditPoints=False
        #该图片的路径
        self.imagePath=None


    def setImagePath(self,imagePath):
        self.imagePath=imagePath



    def setWidget(self,pointsViewer,imageViewer):
        self.pointsViewer=pointsViewer
        self.imageViewer=imageViewer

        #
        self.pointsViewer.rowSelected.connect(self.__setCurrentRow)
        self.pointsViewer.deleteSelected.connect(self.__delCurrentRow)
        self.imageViewer.pointSelect.connect(self.__setCurrentPoint)

        self.pointsNum=self.pointsViewer.groundPointsNum
        self.pointsName=self.pointsViewer.pointsNames

    #设置当前正在选择的控制点编号
    def __setCurrentRow(self,index):
        self.currentID=index
        self.imageViewer.setPointID(index)


    def __setCurrentPoint(self,x,y):
        x=round(x,2)
        y=round(y,2)
        self.pointsViewer.insertPixelTable(x,y)


    def __delCurrentRow(self,index):
        self.imageViewer.deletePoint(index)




    def startSelect(self):      
        if self.pointsNum<1:
            return

        self.imageViewer.setCanSelect(True)
        self.pointsViewer.setCanDelete(True)

        #保存当前选择控制点图片的路径
        info=SegmentImageInfo(self.imageInfoFile)
        info.setImageFilePath(self.imagePath)
        
        if len(self.imageViewer.pointsXYItem)==0:
            self.imageViewer.setPointsName(self.pointsName)

        #在开始之前已经选择了某一行
        if self.pointsViewer.lastRow>-1:
            self.currentID=self.pointsViewer.lastRow
            self.imageViewer.setPointID(self.currentID)

        
        if self.showFlag==False:
            self.imageViewer.showAllPoints(self.pointsViewer.pixelPoints)
            self.showFlag=True

        self.isEditPoints=True


    def endSelect(self):
        #保存文件
        self.pointsViewer.saveAllStatus()
        self.pointsViewer.setCanDelete(False)
        self.imageViewer.setCanSelect(False)

        self.isEditPoints=False


    def showPoints(self):
        if self.showFlag==False:
            self.imageViewer.setPointsName(self.pointsName)
            self.imageViewer.showAllPoints(self.pointsViewer.pixelPoints)
            self.showFlag=True

        else:
            self.imageViewer.hideAllPoints()
            self.showFlag=False

        return self.showFlag








        
