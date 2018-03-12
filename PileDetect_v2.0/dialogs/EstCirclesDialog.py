from PyQt5.QtWidgets import QDialog,QHBoxLayout,QVBoxLayout,QPushButton,QProgressBar,QLabel
from PyQt5.QtCore import Qt,QThread,pyqtSignal 
from functions.CirclesAssign import CirclesAssign

class EstCirclesThread(QThread):
    processImage = pyqtSignal(int)  
    def __int__(self):
        super(EstCirclesThread,self).__init__()


    def setDBFile(self,dbFile):
        self.dbFile=dbFile

    def setGeoMode(self,geoCorrectMode):
        self.geoCorrectMode = geoCorrectMode


    def run(self): 
        self.ass=CirclesAssign(self.dbFile, self.geoCorrectMode)
        self.ass.processImage.connect(self.__process)

        self.ass.run()

        del self.ass


    def __process(self,num):
        self.processImage.emit(num)
        


class EstCirclesDialog(QDialog):
    def __init__(self, geoCorrectMode, parent= None):
        super(EstCirclesDialog,self).__init__()
        self.geoCorrectMode=geoCorrectMode

        if parent is not None:
            self.setWindowIcon(parent.windowIcon())

        self.__initVariable()
        self.__initUI()
        self.resize(500,150)

    def __initVariable(self):
        self.num=0
        self.currentIdx=-1


    def __initUI(self):
        self.setStyleSheet('''
        *{font-size: 15px;}
        QProgressBar{  

                text-align : center ;  
            }  
        ''')
        
        self.setWindowTitle('计算每张图上的基桩坐标')  
        self.setWindowFlags(Qt.WindowCloseButtonHint)

        tipLabel=QLabel('将会删除所有未确认完成图像上已标注的基桩！')

        self.textLabel=QLabel('进度：')
        self.progressBar=QProgressBar()
        startButton=QPushButton('开始')

        hbox=QHBoxLayout()
        hbox.addStretch(1)
        hbox.addWidget(startButton)

        vbox=QVBoxLayout()
        vbox.addSpacing(10)
        vbox.addWidget(tipLabel)
        vbox.addSpacing(10)
        vbox.addWidget(self.textLabel)
        vbox.addSpacing(10)
        vbox.addWidget(self.progressBar)
        vbox.addStretch(1)
        vbox.addLayout(hbox)

        self.setLayout(vbox)

        self.progressBar.setValue(0)
        startButton.clicked.connect(self.__startButton)
    #设置数据库文件
    def setDBFile(self,dbFile):
        self.dbFile=dbFile


    def __startButton(self):
        if self.currentIdx>-1:
            return

        self.estThread=EstCirclesThread()
        self.estThread.setDBFile(self.dbFile)
        self.estThread.setGeoMode(self.geoCorrectMode)
        self.estThread.processImage.connect(self.__updateProgress)

        self.estThread.start()
        # self.estThread.run()


    def __updateProgress(self,num):
        if self.currentIdx<0:
            #计数
            self.currentIdx=0
            #图像个数
            self.num=num

            if self.num==0:
                # self.progressBar.setValue(0)
                self.textLabel.setText("进度：无可用图像")
                return

            # self.progressBar.setMinimum(0)
            self.textLabel.setText("进度：共"+str(num)+"张可计算图像")
            self.progressBar.setMaximum(num)

        else:
            self.currentIdx+=1
            self.progressBar.setValue(num+1)






        



if __name__ == "__main__":  
    import sys  
    from PyQt5.QtWidgets import QApplication
    app = QApplication(sys.argv)  
    qb = EstCirclesDialog()  
    qb.show()  
    sys.exit(app.exec_())
