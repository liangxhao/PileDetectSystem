from PyQt5.QtWidgets import QTableWidget,QMainWindow,QTableWidgetItem,QHBoxLayout,QHeaderView,QDialog,\
    QAbstractItemView,QMenu
from PyQt5.QtCore import Qt


class CircleTable(QDialog):
    def __init__(self, parent=None):
        super(CircleTable,self).__init__()

        if parent is not None:
            self.setWindowIcon(parent.windowIcon())

        self.__initVariable()
        self.__initUI()

    def __initVariable(self):
        self.names=[]
        self.locations=[]
        self.radius=[]

        self.ids=[]


    def __initUI(self):
        self.table=QTableWidget(0,4)
        
        self.table.setHorizontalHeaderLabels(['Name','X','Y','Radius'])
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

        self.setWindowTitle("基桩坐标表")
       
        self.resize(460,500)
        self.setWindowFlags(Qt.WindowCloseButtonHint)


        #设置右键菜单
        self.table.setContextMenuPolicy(Qt.CustomContextMenu)
        self.table.customContextMenuRequested.connect(self.__pixelMenu)



    def __pixelMenu(self,pos):
        try:
            self.table.itemAt(pos).row()
        except:
            return

        rightMenu=QMenu(self.table)
        deleteItem=rightMenu.addAction('删除该行')
        action=rightMenu.exec_(self.table.mapToGlobal(pos))

        if action==deleteItem:
            self.__delRow(self.table.currentRow())


    def __delRow(self,row):
        #移除所要删除的行
        self.table.removeRow(row)#删除该行的显示
        idx=self.ids.pop(row)#删除该行的记录

        self.database.deleteCircleById(idx)




    def __insertTable(self):       
        self.table.setRowCount(len(self.locations))

        if len(self.locations)>0:
            self.table.horizontalHeader().setSectionResizeMode(QHeaderView.ResizeToContents)
        
        for i,(name,loc,r) in enumerate(zip(self.names,self.locations,self.radius)):
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
            #r
            rItem=QTableWidgetItem(str(r))
            rItem.setTextAlignment(Qt.AlignCenter)
            self.table.setItem(i,3,rItem)



    def setContents(self,ids,names,locations,radius):
        self.ids=ids
        self.names=names
        self.locations=locations
        self.radius=radius

        self.__insertTable()



    def setDataBase(self,database):
        self.database=database






                    




if __name__=='__main__':
    
    import sys
    from PyQt5.QtWidgets import QApplication

    
    app = QApplication(sys.argv)
    p=CircleTable()
    p.show()
    sys.exit(app.exec_())
