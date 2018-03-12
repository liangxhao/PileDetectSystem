from PyQt5.QtWidgets import QWidget,QTableWidget,QVBoxLayout,QHBoxLayout,QLabel,QHeaderView,QAbstractItemView,\
    QTableWidgetItem,QMenu,QAction
from PyQt5.QtCore import QThread, pyqtSignal, Qt
from PyQt5.QtGui import QColor,QBrush
from ControlPointsFile import ControlPointsFile
import os

class PointsListView(QWidget):
    rowSelected = pyqtSignal(int)
    deleteSelected=pyqtSignal(int)


    def __init__(self,parent=None):
        super(PointsListView,self).__init__(parent)

        self.__initUI()
        self.__initVariable()



    def setPointsFile(self,saveFile):
        self.__pointFileName=saveFile

        if self.__pointFileName is not None:
            self.__readFile()


    def __initVariable(self):
        self.lastRow=-1
        self.groundPointsNum=0
        self.pixelPointsNum=0

        self.groundPoints=[]#地面控制点 [[x,y]]
        self.pixelPoints=[]#像素控制点 [[x,y]]，没有的地方x=y=''
        self.pointsNames=[]

        self.canDelete=False#是否可以删除像素点

        color=QColor(204,232,207)
        self.__selectBrush=QBrush(color)

        color=QColor(255,255,255)
        self.__defaultBrush=QBrush(color)

    
    #设置是否可以删除像素点
    def setCanDelete(self,flag=False):
        self.canDelete=flag

        


    def __initUI(self):
        #地面控制点表，单位m
        self.ground=QTableWidget(0,3)
        
        self.ground.setHorizontalHeaderLabels(['Name','X(mm)','Y(mm)'])
        self.ground.verticalHeader().setVisible(True)
        self.ground.horizontalHeader().setSectionResizeMode(QHeaderView.Stretch)
        # self.ground.horizontalHeader().setSectionResizeMode(QHeaderView.ResizeToContents)
        self.ground.horizontalHeader().setStretchLastSection(True)
        self.ground.setHorizontalScrollBarPolicy(Qt.ScrollBarAlwaysOff)

        self.ground.setEditTriggers(QAbstractItemView.NoEditTriggers)#不可编辑
        self.ground.setSelectionBehavior(QAbstractItemView.SelectRows)#选择一整行

        self.ground.setRowCount(1)
        self.ground.setItem(0,0,QTableWidgetItem(''))
        self.ground.setItem(0,1,QTableWidgetItem(''))
        self.ground.setItem(0,2,QTableWidgetItem(''))
       


        #图上控制点表，单位像素
        self.pixel=QTableWidget(0,3)
           
        self.pixel.setHorizontalHeaderLabels(['Name','X(pixel)','Y(pixel)'])
        self.pixel.verticalHeader().setVisible(True)
        self.pixel.horizontalHeader().setSectionResizeMode(QHeaderView.Stretch)
        # self.pixel.horizontalHeader().setSectionResizeMode(QHeaderView.ResizeToContents)
        self.pixel.horizontalHeader().setStretchLastSection(True)
        self.pixel.setHorizontalScrollBarPolicy(Qt.ScrollBarAlwaysOff)

        self.pixel.setEditTriggers(QAbstractItemView.NoEditTriggers)#不可编辑
        self.pixel.setSelectionBehavior(QAbstractItemView.SelectRows)#选择一整行

        self.pixel.setRowCount(1)
        self.pixel.setItem(0,0,QTableWidgetItem(''))
        self.pixel.setItem(0,1,QTableWidgetItem(''))
        self.pixel.setItem(0,2,QTableWidgetItem(''))

        ############################################################################

        label1=QLabel('地面控制点',self)
        label2=QLabel('图像控制点',self)
        

        vbox=QVBoxLayout()
        vbox.addWidget(label1)
        vbox.addWidget(self.ground)
        vbox.addWidget(label2)
        vbox.addWidget(self.pixel)

        self.setLayout(vbox)


        self.pixel.setContextMenuPolicy(Qt.CustomContextMenu)
        self.pixel.customContextMenuRequested.connect(self.__pixelMenu)


        self.ground.cellDoubleClicked.connect(self.selectRow)
        self.pixel.cellDoubleClicked.connect(self.selectRow)


    def __pixelMenu(self,pos):
        if not self.canDelete:
            return
        
        #点击在Item之外的话，就跳过
        try:
            self.pixel.itemAt(pos).row()
        except:
            return

        rightMenu=QMenu(self.pixel)
        deleteItem=rightMenu.addAction('清除该行内容')
        action=rightMenu.exec_(self.pixel.mapToGlobal(pos))

        if action==deleteItem:
            self.deleteSelected.emit(self.pixel.currentRow())
            self.__delRow(self.pixel.currentRow())

    #删除掉某一行
    def __delRow(self,index):
        #x
        self.pixel.setItem(index,1,QTableWidgetItem(''))
        #y
        self.pixel.setItem(index,2,QTableWidgetItem(''))
        self.pixelPoints[index]=['','']
        self.pixelPointsNum-=1


            


    def selectRow(self,row,column):
        if self.lastRow==row:
            return
        
        self.__setRowColor(row)
        self.rowSelected.emit(row)


    def __setRowColor(self,row):
        #设置当前选中行的颜色
        for col in range(self.ground.columnCount()):
            #1
            item=self.ground.item(row,col)
            item.setBackground(self.__selectBrush)
            #2
            item=self.pixel.item(row,col)
            item.setBackground(self.__selectBrush)
        
        #恢复默认颜色
        if self.lastRow>-1:     
            for col in range(self.ground.columnCount()):
                #1
                item=self.ground.item(self.lastRow,col)
                item.setBackground(self.__defaultBrush)
                #2
                item=self.pixel.item(self.lastRow,col)
                item.setBackground(self.__defaultBrush)

        self.lastRow=row


    def insertGroundTable(self):
        self.ground.clearContents()
        self.pixel.clearContents()

        self.ground.setRowCount(self.groundPointsNum)
        self.pixel.setRowCount(self.groundPointsNum)

        for i in range(self.groundPointsNum):
            
            nameItem=QTableWidgetItem(str(self.pointsNames[i]))
            nameItem.setTextAlignment(Qt.AlignCenter)

            #地面控制点
            self.ground.setItem(i,0,nameItem)

            (x,y)=self.groundPoints[i]
            #x
            xItem=QTableWidgetItem(str(x))
            xItem.setTextAlignment(Qt.AlignCenter)
            self.ground.setItem(i,1,xItem)
            #y
            yItem=QTableWidgetItem(str(y))
            yItem.setTextAlignment(Qt.AlignCenter)
            self.ground.setItem(i,2,yItem)

            ###############################################
            #图像控制点
            nameItem=QTableWidgetItem(str(self.pointsNames[i]))
            nameItem.setTextAlignment(Qt.AlignCenter)
            self.pixel.setItem(i,0,nameItem)

            (x,y)=self.pixelPoints[i]
            #x
            xItem=QTableWidgetItem(str(x))
            xItem.setTextAlignment(Qt.AlignCenter)
            self.pixel.setItem(i,1,xItem)
            #y
            yItem=QTableWidgetItem(str(y))
            yItem.setTextAlignment(Qt.AlignCenter)
            self.pixel.setItem(i,2,yItem)



    def insertPixelTable(self,x,y):    
        #x
        xItem=QTableWidgetItem(str(x))
        xItem.setTextAlignment(Qt.AlignCenter)
        self.pixel.setItem(self.lastRow,1,xItem)
        #x
        yItem=QTableWidgetItem(str(y))
        yItem.setTextAlignment(Qt.AlignCenter)
        self.pixel.setItem(self.lastRow,2,yItem)

        self.pixelPoints[self.lastRow]=[x,y]

        self.__sumPointsNum()



    def __sumPointsNum(self):
        self.pixelPointsNum=len([item for item in self.pixelPoints if item[0]!=''])

        # for item in self.pixelPoints:
        #     if item[0]!='':
        #         self.pixelPointsNum+=1

    def __readFile(self):
        cp=ControlPointsFile(self.__pointFileName)
        (self.groundPoints,self.pixelPoints,self.pointsNames)=cp.getPoints()
        self.groundPointsNum=len(self.groundPoints)
        self.__sumPointsNum()

        if self.groundPointsNum>0:
            self.insertGroundTable()



    def __saveFile(self):
        cp=ControlPointsFile(self.__pointFileName)
        cp.savePixelPoints(self.pixelPoints)

   

    def saveAllStatus(self):
        self.__saveFile()




        

        





        









if __name__ == "__main__":
    import sys
    from PyQt5.QtWidgets import  QApplication
    app = QApplication(sys.argv)
    
    w = PointsListView()
    w.show()
    sys.exit(app.exec_())