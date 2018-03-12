from PyQt5.QtWidgets import QDialog,QHBoxLayout,QVBoxLayout,QPushButton,QProgressBar,QLabel
from PyQt5.QtCore import Qt,QThread,pyqtSignal 
from access.DataBase import DataBase
from widgets.ReportTable import ReportTable

class CollectThread(QThread):
    processStep = pyqtSignal(int)  
    def __int__(self):  
        super(CollectThread,self).__init__()  


    def setDBFile(self,dbFile):
        self.dbFile=dbFile


    def run(self): 
        #连接数据库
        database=DataBase(self.dbFile)
        database.connectDB()
        self.processStep.emit(1)

        #统计查询结果
        ids,gPoints,grs=database.getCollectCircles()

        self.processStep.emit(2)
        #更新存入结果
        
        database.updateCollectCircles(ids,gPoints,grs)
        database.closeDB()

        self.processStep.emit(3)

        del database





class CollCirclesDialog(QDialog):
    def __init__(self, parent= None):  
        super(CollCirclesDialog,self).__init__()
        if parent is not None:
            self.setWindowIcon(parent.windowIcon())

        self.__initVariable()
        self.__initUI()
        self.resize(500,150)

    def __initVariable(self):
        self.dbFile=None
        self.collThread=None

        self.canlook=False


    def __initUI(self):
        self.setStyleSheet('''
        *{font-size: 15px;}
        QProgressBar{  

                text-align : center ;  
            }  
        ''')
        
        self.setWindowTitle('统计结果')  
        self.setWindowFlags(Qt.WindowCloseButtonHint)
        self.textLabel=QLabel('进度：')
        self.progressBar=QProgressBar()

        startButton=QPushButton('开始')
        lookReportButton=QPushButton("查看")

        hbox=QHBoxLayout()
        hbox.addStretch(1)
        hbox.addWidget(startButton)
        hbox.addSpacing(10)
        hbox.addWidget(lookReportButton)

        vbox=QVBoxLayout()
        vbox.addSpacing(10)
        vbox.addWidget(self.textLabel)
        vbox.addSpacing(10)
        vbox.addWidget(self.progressBar)
        vbox.addStretch(1)
        vbox.addLayout(hbox)

        self.setLayout(vbox)

        self.progressBar.setMaximum(3)
        self.progressBar.setValue(0)
        startButton.clicked.connect(self.__startButton)
        lookReportButton.clicked.connect(self.__lookReport)


    #设置数据库文件
    def setDBFile(self,dbFile):
        self.dbFile=dbFile


    def __startButton(self):
        if self.collThread is not None:
            return

        self.canlook = False
        self.collThread=CollectThread()
        self.collThread.setDBFile(self.dbFile)
        self.collThread.processStep.connect(self.__updateProgress)

        self.collThread.start()


    def __updateProgress(self,num):
        self.progressBar.setValue(num)

        if num==1:
            self.textLabel.setText("进度：正在统计数据...")
        elif num==2:
            self.textLabel.setText("进度：正在更新数据...")
        else:
            self.textLabel.setText("进度：完成！")
            self.canlook=True


    def __lookReport(self):
        if not self.canlook:
            return

        database=DataBase(self.dbFile)
        database.connectDB()
        names,errors=database.getReportWithoutCoords()
        database.closeDB()

        table=ReportTable(self)
        table.setContents(names,errors)
        table.exec_()





        




if __name__ == "__main__":  
    import sys  
    from PyQt5.QtWidgets import QApplication
    app = QApplication(sys.argv)  
    qb = CollCirclesDialog()  
    qb.show()  
    sys.exit(app.exec_())
