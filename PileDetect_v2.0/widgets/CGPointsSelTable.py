from PyQt5.QtWidgets import QWidget,QTableWidget,QVBoxLayout,QLabel,QHeaderView,QAbstractItemView,\
    QTableWidgetItem
from PyQt5.QtCore import pyqtSignal, Qt
from PyQt5.QtGui import QColor,QBrush


class CGPointsSelTable(QWidget):
    idSelected = pyqtSignal(int)

    def __init__(self):
        super(CGPointsSelTable,self).__init__()

        self.__initVariable()
        self.__initUI()

    def __initVariable(self):
        self.ids=None
        # self.names=None
        # self.points=None

        self.__lastRow=-1



    def __initUI(self):
        #地面控制点表，单位m
        self.ground=QTableWidget(0,3)
        
        self.ground.setHorizontalHeaderLabels(['Name','X(mm)','Y(mm)'])
        self.ground.verticalHeader().setVisible(True)
        self.ground.verticalHeader().setDefaultSectionSize(25)

        self.ground.horizontalHeader().setSectionResizeMode(QHeaderView.Stretch)
        
        # self.ground.horizontalHeader().setSectionResizeMode(QHeaderView.ResizeToContents)
        # self.ground.horizontalHeader().setSectionResizeMode(QHeaderView.Interactive)

        self.ground.horizontalHeader().setStretchLastSection(True)
        self.ground.setHorizontalScrollBarPolicy(Qt.ScrollBarAlwaysOff)

        self.ground.setEditTriggers(QAbstractItemView.NoEditTriggers)#不可编辑
        self.ground.setSelectionBehavior(QAbstractItemView.SelectRows)#选择一整行


        label=QLabel('地面控制点',self)
        
        vbox=QVBoxLayout()
        vbox.addWidget(label)
        vbox.addWidget(self.ground)


        self.setStyleSheet("""
            QLabel{
                font-family: Microsoft YaHei;
            }
        """)

        self.setLayout(vbox)

        self.ground.cellDoubleClicked.connect(self.__selectRow)



    def setPoints(self,ids,names,points):
        self.ids=ids
        # self.names=names
        # self.points=points

        self.ground.setRowCount(len(self.ids))

        for i,(name,point) in enumerate(zip(names,points)):
            nameItem=QTableWidgetItem(name)
            nameItem.setTextAlignment(Qt.AlignCenter)
            self.ground.setItem(i,0,nameItem)

            (x,y)=point
            #x
            xItem=QTableWidgetItem(str(x))
            xItem.setTextAlignment(Qt.AlignCenter)
            self.ground.setItem(i,1,xItem)
            #y
            yItem=QTableWidgetItem(str(y))
            yItem.setTextAlignment(Qt.AlignCenter)
            self.ground.setItem(i,2,yItem)



    def __selectRow(self,row,column):
        if self.__lastRow==row:
            return
        #发送被选中点的id号
        self.idSelected.emit(self.ids[row])

        self.__lastRow=row


            





if __name__=='__main__':

    import sys
    from PyQt5.QtWidgets import QApplication
    from MainProgram import MainProgram

    ids=[1,2,3]
    names=['11','22','33']
    points=[[12.34,323],[32,45],[323,55]]

    app = QApplication(sys.argv)
    program=CGPointsSelTable()
    program.setPoints(ids,names,points)
    program.show()
    
    
    sys.exit(app.exec_())