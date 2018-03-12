from PyQt5.QtWidgets import QWidget,QTableWidget,QVBoxLayout,QLabel,QHeaderView,QAbstractItemView,\
    QTableWidgetItem,QMenu,QAction
from PyQt5.QtCore import pyqtSignal, Qt
from PyQt5.QtGui import QColor,QBrush
import copy

class CPPointsSelTable(QWidget):
    idSelected = pyqtSignal(int)
    idDeleted=pyqtSignal(int)

    def __init__(self):
        super(CPPointsSelTable,self).__init__()

        self.__initVariable()
        self.__initUI()

    def __initVariable(self):
        self.ids=[]
        # self.names=None
        # self.points=None
        # self.pErrors=None
        # self.gErrors=None

        self.__lastRow=-1

        color=QColor(204,232,207)
        self.__selectBrush=QBrush(color)

        color=QColor(255,255,255)
        self.__defaultBrush=QBrush(color)



    def __initUI(self):
        #地面控制点表，单位m
        self.pixel=QTableWidget(0,5)
        
        self.pixel.setHorizontalHeaderLabels(['Name','X','Y','Error(pixel)','Error(mm)'])
        self.pixel.verticalHeader().setVisible(True)
        self.pixel.verticalHeader().setDefaultSectionSize(25)

        self.pixel.horizontalHeader().setSectionResizeMode(QHeaderView.Stretch)
        # self.pixel.horizontalHeader().setSectionResizeMode(QHeaderView.ResizeToContents)
        # self.pixel.horizontalHeader().setSectionResizeMode(QHeaderView.Interactive)
        self.pixel.horizontalHeader().setStretchLastSection(True)
        self.pixel.setHorizontalScrollBarPolicy(Qt.ScrollBarAlwaysOff)


        self.pixel.setEditTriggers(QAbstractItemView.NoEditTriggers)#不可编辑
        self.pixel.setSelectionBehavior(QAbstractItemView.SelectRows)#选择一整行


        label=QLabel('选择像素点',self)
        
        vbox=QVBoxLayout()
        vbox.addWidget(label)
        vbox.addWidget(self.pixel)


        self.setStyleSheet("""
            QLabel{
                font-family: Microsoft YaHei;
            }
        """)

        self.setLayout(vbox)
        self.pixel.cellDoubleClicked.connect(self.__selectRow)

        self.pixel.setContextMenuPolicy(Qt.CustomContextMenu)
        self.pixel.customContextMenuRequested.connect(self.__pixelMenu)



    def setPoints(self,ids,names,points,pErrors,gErrors):
        #为了不影响原来的值，这里采用深拷贝
        self.ids=copy.deepcopy(ids)
        # self.names=names
        # self.points=points
        # self.pErrors=pErrors
        # self.gErrors=gErrors

        self.pixel.setRowCount(len(self.ids))

        for i,(name,point,pe,ge) in enumerate(zip(names,points,pErrors,gErrors)):
            if point[0] is not None:
                x,y=point
            else:
                x,y=['','']

            if pe is None:
                pe,ge=['','']     
             
            self.__setRowContent(i,name,x,y,pe,ge)


    #设置行内容
    def __setRowContent(self,row,name,x,y,pe,ge):
        #name
        if name is not None:
            nameItem=QTableWidgetItem(name)
            nameItem.setTextAlignment(Qt.AlignCenter)
            self.pixel.setItem(row,0,nameItem)
        #x
        if x is not None:
            xItem=QTableWidgetItem(str(x))
            xItem.setTextAlignment(Qt.AlignCenter)
            self.pixel.setItem(row,1,xItem)
        #y
        if y is not None:
            yItem=QTableWidgetItem(str(y))
            yItem.setTextAlignment(Qt.AlignCenter)
            self.pixel.setItem(row,2,yItem)

        #pe
        if pe is not None:
            pItem=QTableWidgetItem(str(pe))
            pItem.setTextAlignment(Qt.AlignCenter)
            self.pixel.setItem(row,3,pItem)
        #ge
        if ge is not None:
            gItem=QTableWidgetItem(str(ge))
            gItem.setTextAlignment(Qt.AlignCenter)
            self.pixel.setItem(row,4,gItem)
        


    #右键菜单
    def __pixelMenu(self,pos):
        try:
            self.pixel.itemAt(pos).row()
        except:
            return

        rightMenu=QMenu(self.pixel)
        deleteItem=rightMenu.addAction('删除该行')
        action=rightMenu.exec_(self.pixel.mapToGlobal(pos))

        if action==deleteItem:
            self.__delRow(self.pixel.currentRow())



    def __delRow(self,row):
        #先重置被当前被选中的行
        self.__setRowDefaultColor()
        self.__lastRow=-1

        #移除所要删除的行
        self.pixel.removeRow(row)#删除该行的显示
        idx=self.ids.pop(row)#删除该行的记录

        self.idDeleted.emit(idx)#发送删除的点号

        if len(self.ids)<3:
            self.clearError()#删除至点太少时，清除误差


    #新增一行
    def insertPoint(self,idx,name):
        self.ids.append(idx)
        rowNum=self.pixel.rowCount()
        self.pixel.insertRow(rowNum)

        self.__setRowContent(rowNum,name,'','','','')

        self.clearError()

    #设置该点的像素坐标
    def setPointCoords(self,idx,x,y):
        #idx所在的行应该就是self.__lastRow，这里不再做检查
        if self.__lastRow<0:
            return
        self.__setRowContent(self.__lastRow,None,x,y,None,None)
        #设置颜色
        self.__setRowColor(self.__lastRow,[1,2])


    #设置坐标点的误差
    def setPointError(self,idxs,pErrors,gErrors):
        if len(pErrors)!=self.pixel.rowCount():
            raise Exception('the length of error is unvalid!')
        #ids与row的顺序应该是对应的，这里不再做检查
        for row,(pe,ge) in enumerate(zip(pErrors,gErrors)):
            self.__setRowContent(row,None,None,None,pe,ge)

        #设置颜色保持
        if self.__lastRow>-1:
            self.__setRowColor(self.__lastRow,[3,4])




    #双击某一行后的执行事件
    def __selectRow(self,row,column):
        if self.__lastRow==row:
            return
        #发送被选中点的id号
        self.idSelected.emit(self.ids[row])    
        #修改颜色  
        self.__setRowColor(row,range(0,5))
        self.__setRowDefaultColor()

        self.__lastRow=row

    
    #设置当前编辑的行的颜色
    def __setRowColor(self,row,cols):
        #设置当前选中行的颜色
        for col in cols:
            item=self.pixel.item(row,col)
            item.setBackground(self.__selectBrush)
   
        

    #上一次编辑行的恢复默认颜色
    def __setRowDefaultColor(self):
        #恢复默认颜色
        if self.__lastRow>-1:     
            for col in range(self.pixel.columnCount()):
                item=self.pixel.item(self.__lastRow,col)
                item.setBackground(self.__defaultBrush)


    #清除掉所有误差项
    def clearError(self):
        #当点个数太少时无法计算误差，清除之
        num=self.pixel.rowCount()

        for i in range(num):
            self.__setRowContent(i,None,None,None,'','')

        #设置颜色保持
        if self.__lastRow>-1:
            self.__setRowColor(self.__lastRow,[3,4])
        



            





if __name__=='__main__':
    import sys
    from PyQt5.QtWidgets import QApplication

    ids=[1,2,3]
    names=['11','22','33']
    points=[[12.34,323],[32,45],[323,55]]
    pe=[12,334,23]
    ge=[13,45,65]

    app = QApplication(sys.argv)
    program=CPPointsSelTable()
    program.setPoints(ids,names,points,pe,ge)
    # program.setPointError([],[],[])
    program.insertPoint(1,'22')
    program.show()
    
    
    sys.exit(app.exec_())