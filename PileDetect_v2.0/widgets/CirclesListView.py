from PyQt5.QtWidgets import QWidget,QTableWidget,QVBoxLayout,QHBoxLayout,QLabel,QHeaderView,QAbstractItemView,\
    QTableWidgetItem,QMenu,QAction,QPushButton
from PyQt5.QtCore import pyqtSignal, Qt
from PyQt5.QtGui import QColor,QBrush


class CirclesListView(QWidget):
    selectPoint = pyqtSignal(int)
    deletePoint=pyqtSignal(int)
    stateUpdated=pyqtSignal(int)
    segmentCall=pyqtSignal()

    def __init__(self):
        super(CirclesListView,self).__init__()

        self.__initVariable()
        self.__initUI()

    def __initVariable(self):
        #所有点
        self.ids=[]
        self.gErrors=None


        self.__lastRow=-1

        color=QColor(204,232,207)
        self.__selectBrush=QBrush(color)
        color=QColor(255,255,255)
        self.__defaultBrush=QBrush(color)

        self.text="标注基桩"

        self.state=0



    def __initUI(self):
        #地面控制点表，单位m
        self.pixel=QTableWidget(0,2)
        
        self.pixel.setHorizontalHeaderLabels(['Name','Error(mm)'])
        self.pixel.verticalHeader().setVisible(True)
        self.pixel.verticalHeader().setDefaultSectionSize(25)

        self.pixel.horizontalHeader().setSectionResizeMode(QHeaderView.Stretch)
        # self.pixel.horizontalHeader().setSectionResizeMode(QHeaderView.ResizeToContents)
        # self.pixel.horizontalHeader().setSectionResizeMode(QHeaderView.Interactive)
        self.pixel.horizontalHeader().setStretchLastSection(True)
        self.pixel.setHorizontalScrollBarPolicy(Qt.ScrollBarAlwaysOff)


        self.pixel.setEditTriggers(QAbstractItemView.NoEditTriggers)#不可编辑
        self.pixel.setSelectionBehavior(QAbstractItemView.SelectRows)#选择一整行


        self.stateLable=QLabel(self.text)
        self.completedButton=QPushButton('')
        self.completedButton.hide()
        self.completedButton.clicked.connect(self.__updateState)

        self.segmentButton=QPushButton('自动识别')
        self.segmentButton.clicked.connect(self.__segmentCircle)


        hbox=QHBoxLayout()
        hbox.addWidget(self.stateLable)
        hbox.addStretch(1)
        hbox.addWidget(self.segmentButton)
        hbox.addStretch(1)
        hbox.addWidget(self.completedButton)
        
        vbox=QVBoxLayout()
        vbox.addLayout(hbox)
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


    def setState(self,state):
        if state==0:
            self.stateLable.setText(self.text+'（未选择完成）')
            self.completedButton.setText("确认完成")         
        else:
            self.stateLable.setText(self.text+'（已选择完成）')
            self.completedButton.setText("取消完成")
        self.state=state

        self.completedButton.show()

    def __updateState(self):
        if self.state==0:
            self.state=1
        else:
            self.state=0
        self.setState(self.state)
        self.stateUpdated.emit(self.state)


    def __segmentCircle(self):
        self.segmentCall.emit()



    def setPoints(self,ids,names,gErrors):
        self.ids=ids
        # self.pPoints=points
        self.gErrors=gErrors

        self.pixel.setRowCount(len(self.ids))

        for i,(name,ge) in enumerate(zip(names,gErrors)):
            if ge is None:
                ge=''
            self.__setRowContent(i,name,ge)


    #设置行内容
    def __setRowContent(self,row,name,ge):
        #name
        if name is not None:
            nameItem=QTableWidgetItem(name)
            nameItem.setTextAlignment(Qt.AlignCenter)
            self.pixel.setItem(row,0,nameItem)
        #ge
        if ge is not None:
            if ge !='':
                ge=round(ge,3)
            gItem=QTableWidgetItem(str(ge))
            gItem.setTextAlignment(Qt.AlignCenter)
            self.pixel.setItem(row,1,gItem)
        


    #右键菜单
    def __pixelMenu(self,pos):
        try:
            self.pixel.itemAt(pos).row()
        except:
            return

        rightMenu=QMenu(self.pixel)
        deleteItem=rightMenu.addAction('清除该行')
        action=rightMenu.exec_(self.pixel.mapToGlobal(pos))

        if action==deleteItem:
            self.__delRow(self.pixel.currentRow())



    def __delRow(self,row):
        #先重置被当前被选中的行
        idx=self.ids[row]

        self.clearError(row)#删除至点太少时，清除误差
        self.gErrors[row]=None
        # self.pPoints[idx]=None

        self.deletePoint.emit(idx)#发送删除的点号

    
    def setPoint(self,idx,gError):
        #idx应该就是当前行，这里不再做检查
        row=self.ids.index(idx)

        self.gErrors[row]=gError

        #设置坐标点的误差显示
        self.__setRowContent(row,None,gError)

        #设置颜色保持
        self.__setRowColor(row,[1,])


    #双击某一行后的执行事件
    def __selectRow(self,row,column=-1):
        if self.__lastRow==row:
            return
        #发送被选中点的id号
        self.selectPoint.emit(self.ids[row])    
        #修改颜色  
        if self.__lastRow!=row:
            self.__setRowColor(row,range(0,2))
            self.__setRowDefaultColor()

            self.__lastRow=row

    #选择某个圆
    def selectCircle(self,idx):
        row=self.ids.index(idx)
        self.__selectRow(row)



    # 清除之前的选择
    def clearSelect(self):
        self.__setRowDefaultColor()
        self.__lastRow=-1


    
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
    def clearError(self,row):
        #当点个数太少时无法计算误差，清除之
        self.__setRowContent(row,None,'')
        #设置颜色保持
        if self.__lastRow==row:
            self.__setRowColor(self.__lastRow,[1,])
        



if __name__=='__main__':
    import sys
    from PyQt5.QtWidgets import QApplication

    ids=[1,2,3]
    names=['11','22','33']
    ge=[13,45,65]

    app = QApplication(sys.argv)
    program=CirclesListView()
    program.setPoints(ids,names,ge)
    program.setState(0)
    # program.setPointError([],[],[])
    # program.insertPoint(1,'22')
    program.show()
    
    
    sys.exit(app.exec_())