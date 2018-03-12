from PyQt5.QtWidgets import QTableWidget,QMainWindow,QTableWidgetItem,QHBoxLayout,QHeaderView,QDialog,\
    QAbstractItemView
from PyQt5.QtCore import Qt


class ReportTable(QDialog):
    def __init__(self, parent=None):
        super(ReportTable,self).__init__()
        if parent is not None:
            self.setWindowIcon(parent.windowIcon())

        self.__initVariable()
        self.__initUI()

    def __initVariable(self):
        self.names=[]
        self.errors=[]



    def __initUI(self):
        self.table=QTableWidget(0,4)
        
        self.table.setHorizontalHeaderLabels(['Name','error-X','error-Y','error-R'])
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

        self.setWindowTitle("桩偏位表")
       
        self.resize(460,500)
        self.setWindowFlags(Qt.WindowCloseButtonHint)
        

    def __insertTable(self):       
        self.table.setRowCount(len(self.names))

        if len(self.names)>0:
            self.table.horizontalHeader().setSectionResizeMode(QHeaderView.ResizeToContents)
        
        for i,(name,error) in enumerate(zip(self.names,self.errors)):
            (x,y,r)=error

            if x is None:
                x,y,r='','',''

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
            # r
            rItem = QTableWidgetItem(str(r))
            rItem.setTextAlignment(Qt.AlignCenter)
            self.table.setItem(i, 3, rItem)
            



    def setContents(self,names,errors):
        self.names=names
        self.errors=errors


        self.__insertTable()



                    




if __name__=='__main__':
    
    import sys
    from PyQt5.QtWidgets import QApplication

    
    app = QApplication(sys.argv)
    p=ReportTable()
    p.show()
    
    sys.exit(app.exec_())
