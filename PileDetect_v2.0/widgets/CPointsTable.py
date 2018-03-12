from PyQt5.QtWidgets import QTableWidget,QMainWindow,QTableWidgetItem,QHBoxLayout,QHeaderView,QDialog,\
    QAbstractItemView
from PyQt5.QtCore import Qt


class CPointsTable(QDialog):
    def __init__(self, parent=None):
        super(CPointsTable,self).__init__()
        
        self.setWindowIcon(parent.windowIcon())

        self.__initVariable()
        self.__initUI()

    def __initVariable(self):
        self.names=[]
        self.points=[]



    def __initUI(self):

        self.table=QTableWidget(0,3)
        
        self.table.setHorizontalHeaderLabels(['Name','X','Y'])
        self.table.verticalHeader().setVisible(True)
        self.table.horizontalHeader().setSectionResizeMode(QHeaderView.Stretch)
        # self.table.horizontalHeader().setSectionResizeMode(QHeaderView.ResizeToContents)
        self.table.horizontalHeader().setStretchLastSection(True)
        self.table.setHorizontalScrollBarPolicy(Qt.ScrollBarAlwaysOff)

        self.table.setEditTriggers(QAbstractItemView.NoEditTriggers)#不可编辑
        self.table.setSelectionBehavior(QAbstractItemView.SelectRows)#选择一整行

        
        

        hbox = QHBoxLayout()
        hbox.addWidget(self.table)
        self.setLayout(hbox)

        self.setWindowTitle("地面控制点坐标")
       
        self.resize(460,500)
        self.setWindowFlags(Qt.WindowCloseButtonHint)
        

    def __insertTable(self):       
        self.table.setRowCount(len(self.points))

        if len(self.points)>0:
            self.table.horizontalHeader().setSectionResizeMode(QHeaderView.ResizeToContents)
        
        for i,(name,loc) in enumerate(zip(self.names,self.points)):
            (x,y)=loc

            nameItem=QTableWidgetItem(name)
            nameItem.setTextAlignment(Qt.AlignCenter)
            self.table.setItem(i,0,nameItem)
            #x
            xItem=QTableWidgetItem(str(x))
            xItem.setTextAlignment(Qt.AlignCenter)
            self.table.setItem(i,1,xItem)
            #y
            yItem=QTableWidgetItem(str(y))
            yItem.setTextAlignment(Qt.AlignCenter)
            self.table.setItem(i,2,yItem)




    def setContents(self,names,points):
        self.names=names
        self.points=points

        self.__insertTable()




