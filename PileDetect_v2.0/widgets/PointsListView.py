from PyQt5.QtWidgets import QWidget,QVBoxLayout
from PyQt5.QtCore import pyqtSignal, Qt

from widgets.CGPointsSelTable import CGPointsSelTable
from widgets.CPPointsSelTable import CPPointsSelTable

class PointsListView(QWidget):
    deletePoint=pyqtSignal(int)
    addPoint=pyqtSignal(int,str)
    selectPoint=pyqtSignal(int)

    def __init__(self):
        super(PointsListView,self).__init__()
        self.__initVariable()
        self.__initUI()

    def __initVariable(self):
        self.idGs=None#地面点的id
        self.names=None#地面点点名
        self.gPoints=None#地面点坐标

        self.idPs=None#像素点的id
        self.pPoints=None#像素点坐标
        self.pErrors=None#像素级的误差
        self.gErrors=None#地面坐标级的误差

        self.currentID=-1



    def __initUI(self):
        self.gTable=CGPointsSelTable()
        self.pTable=CPPointsSelTable()

        vbox=QVBoxLayout()
        vbox.addWidget(self.gTable)
        vbox.addWidget(self.pTable)

        self.setLayout(vbox)


        self.gTable.idSelected.connect(self.__addPoint)
        self.pTable.idSelected.connect(self.__setEditID)
        self.pTable.idDeleted.connect(self.__deletePoint)


    #设置显示所有的地面控制点信息
    def setGroundPoints(self,ids,names,points):
        self.idGs=ids
        self.names=names
        self.gPoints=points

        self.gTable.setPoints(self.idGs,self.names,self.gPoints)

    #设置显示之前已经选择过的像素点坐标
    def setPixelPoints(self,ids,names,points,pErrors,gErrors):
        self.idPs=ids
        self.pPoints=points

        self.pTable.setPoints(self.idPs,names,self.pPoints,pErrors,gErrors)



    #从地面控制点中新增一个点到像素中供选择
    def __addPoint(self,idx):
        if idx in self.idPs:
            return
        
        pos=self.idGs.index(idx)
        name=self.names[pos]

        #在表中增加一个显示
        self.pTable.insertPoint(idx,name)
        #增加一条记录
        self.idPs.append(idx)#按顺序记录id号
        self.pPoints.append([None,None])#初始化为None

        #将新增点的id和name发送出去，让imageView知道
        self.addPoint.emit(idx,name)


    #从记录中删除某个点
    def __deletePoint(self,idx):
        if idx==self.currentID:
            self.currentID=-1
        #先找到存储位置
        pos=self.idPs.index(idx)
        #分别删除
        self.idPs.pop(pos)
        self.pPoints.pop(pos)

        #发送出去删除的点号，让View接受到，再图像上删除之
        self.deletePoint.emit(idx)


    #设置当前正在编辑的点的id
    def __setEditID(self,idx):
        self.currentID=idx
        self.selectPoint.emit(idx)
    
    #图像上点某个点后，获得坐标，在列表中进行更新
    def setCurrentCoords(self,x,y):
        if self.currentID<0:
            return
        pos=self.idPs.index(self.currentID)
        self.pPoints[pos]=[x,y]

        self.pTable.setPointCoords(self.currentID,x,y)

    #设置误差值
    def setErrors(self,pErrors,gErrors):
        self.pErrors=pErrors
        self.gErrors=gErrors
        if pErrors is not None:
            self.pTable.setPointError(self.idPs,pErrors,gErrors)

    # #清除误差项
    # def clearError(self):
    #     self.pTable.clearError()











    



if __name__=='__main__':

    import sys
    from PyQt5.QtWidgets import QApplication


    app = QApplication(sys.argv)
    program=PointsListView()

    program.show()
    
    
    sys.exit(app.exec_())
