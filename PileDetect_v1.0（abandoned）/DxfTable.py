from PyQt5.QtWidgets import QTableWidget,QMainWindow,QTableWidgetItem,QHBoxLayout,QHeaderView,QDialog,\
    QAbstractItemView
from PyQt5.QtCore import Qt
from CircleObjFile import CircleObjFile

class DxfTable(QDialog):
    def __init__(self, file, parent=None):
        super(DxfTable,self).__init__(parent)
        self.file=file

        self.__initVariable()
        self.__initUI()

    def __initVariable(self):
        self.names=[]
        self.location=[]
        self.radius=[]


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

        
        

        self.__importFile()
        self.__insertTable()

        hbox = QHBoxLayout()
        hbox.addWidget(self.table)
        self.setLayout(hbox)

        self.setWindowTitle("桩坐标表")
       
        self.resize(460,500)
        self.setWindowFlags(Qt.WindowCloseButtonHint)
        self.exec_()

    def __insertTable(self):       
        self.table.setRowCount(len(self.location))

        if len(self.location)>0:
            self.table.horizontalHeader().setSectionResizeMode(QHeaderView.ResizeToContents)
        
        for i,(name,loc,r) in enumerate(zip(self.names,self.location,self.radius)):
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



    def __importFile(self):
        self.names.clear()
        self.location.clear()
        self.radius.clear()

        cof=CircleObjFile(self.file)
        (self.names,self.location,self.radius)=cof.getObjCircleFromGround()



                    




if __name__=='__main__':
    
    import sys
    from PyQt5.QtWidgets import QApplication
    file='G://Python//Project//dxf.txt'
    
    app = QApplication(sys.argv)
    p=DxfTable(file)
    
    sys.exit(app.exec_())
