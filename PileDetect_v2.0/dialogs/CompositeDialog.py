from PyQt5.QtWidgets import QDialog,QHBoxLayout,QVBoxLayout,QPushButton,QProgressBar,QLabel,QLineEdit,QFileDialog
from PyQt5.QtCore import Qt,QThread,pyqtSignal 

from functions.Composite import Composite
import os

class CompositeDialog(QDialog):
    def __init__(self, parent= None):  
        super(CompositeDialog,self).__init__()
        if parent is not None:
            self.setWindowIcon(parent.windowIcon())

        self.__initVariable()
        self.__initUI()
        self.resize(500,220)

    def __initVariable(self):
        # #相机的标定文件
        # self.cameraFile=None
        # #图像叠加后的图片名称
        # self.compositeImage=None

        #exe文件
        self.programFile=None
        #工程文件
        self.projectFile=None

        ####
        self.comp=None

        self.numImages=0

        self.isFirst=True

        self.geoCorrectMode=0


    #需要用到的一些文件
    def setParms(self,programFile,projectFile):
        self.programFile=programFile
        self.projectFile=projectFile

    def setGeoCorrectMode(self,mode):
        self.geoCorrectMode=mode


    def __initUI(self):
        self.setStyleSheet('''
        *{font-size: 15px;}
        QProgressBar{  
                text-align : center ;  
            }
        QLineEdit{
            background-color : rgb(255,255,255)
        }

        ''')
        
        self.setWindowTitle('导出图像')
        
        #
        imageButton=QPushButton('保存路径')
        self.imageEdit=QLineEdit('')
        self.imageEdit.setEnabled(False)

        hbox1=QHBoxLayout()
        hbox1.addWidget(imageButton)
        hbox1.addWidget(self.imageEdit)
        hbox1.setStretchFactor(self.imageEdit,3)
        hbox1.addWidget(QLabel('    '))
        hbox1.addStretch(1)

        #
        # cameraButton=QPushButton('相机文件')
        # self.cameraEdit=QLineEdit('')
        # self.cameraEdit.setEnabled(False)
        #
        #
        # hbox2=QHBoxLayout()
        # hbox2.addWidget(cameraButton)
        # hbox2.addWidget(self.cameraEdit)
        # hbox2.setStretchFactor(self.cameraEdit,3)
        # hbox2.addWidget(QLabel('可选'))
        # hbox2.addStretch(1)

        #
        self.textLabel=QLabel('进度：')

        hbox3=QHBoxLayout()
        hbox3.addWidget(self.textLabel)
        hbox3.addStretch(1)

        #
        self.progressBar=QProgressBar()
        self.progressBar.setValue(0)

        #
        startButton=QPushButton('开始')

        hbox4=QHBoxLayout()
        hbox4.addStretch(1)
        hbox4.addWidget(startButton)

        ###########
        vbox=QVBoxLayout()

        vbox.addSpacing(15)
        vbox.addLayout(hbox1)

        # vbox.addSpacing(10)
        # vbox.addLayout(hbox2)

        vbox.addSpacing(10)
        vbox.addLayout(hbox3)

        vbox.addSpacing(0)
        vbox.addWidget(self.progressBar)

        vbox.addSpacing(10)
        vbox.addLayout(hbox4)

        self.setLayout(vbox)
        self.setWindowFlags(Qt.WindowCloseButtonHint)

        imageButton.clicked.connect(self.__setImageFile)
        # cameraButton.clicked.connect(self.__setCameraFile)
        startButton.clicked.connect(self.__start)


    #设置保存文件
    def __setImageFile(self):
        compositeImage, ext=QFileDialog.getSaveFileName(self,
                                                    "保存拼接图像",
                                                    '',
                                                    "TIF (*.tif)")
        if len(compositeImage)>0:
            self.imageEdit.setText(compositeImage)


    #设置相机文件
    # def __setCameraFile(self):
    #     cameraFile, ext = QFileDialog.getOpenFileName(self,
    #                                 "选择相机参数文件",
    #                                 '',
    #                                 "Camera Files (*.xml)")
    #
    #     if len(cameraFile)>0:
    #         self.cameraEdit.setText(cameraFile)


    #开始运行拼接
    def __start(self):
        if self.comp is not None:
            return

        compositeImage=self.imageEdit.text()

        if len(compositeImage)==0:
            return
 
        # cameraFile=self.cameraEdit.text()
        #
        # if not os.path.isfile(cameraFile):
        #     cameraFile=None

        self.comp=Composite(self.projectFile)
        self.comp.setProgram(self.programFile)
        self.comp.setGeoCorrectMode(self.geoCorrectMode)
        self.comp.setCompositeInfo(compositeImage)

        self.comp.processStep.connect(self.__updateProgress)

        # self.comp.start()
        self.comp.run()



    def __updateProgress(self,num):
        if self.isFirst:
            if num==0:
                self.textLabel.setText('进度：没有可用图像')
            else:
                self.progressBar.setMaximum(num+1)
                self.progressBar.setValue(0)

            self.numImages=num
            self.isFirst=False

        else:
            self.progressBar.setValue(num+1)

            if self.numImages == num:
                self.isFirst=True
                del self.comp
                self.comp=None



        

        

        





if __name__ == "__main__":  
    import sys  
    from PyQt5.QtWidgets import QApplication
    app = QApplication(sys.argv)  
    qb = CompositeDialog()  
    qb.show()  
    sys.exit(app.exec_())