from PyQt5.QtWidgets import QFileDialog,QHBoxLayout,QVBoxLayout,QLabel,QPushButton,QLineEdit,QMessageBox,QDialog,QProgressBar 
from PyQt5.QtCore import Qt,QThread,pyqtSignal
from access.DxfReader import DxfReader
from access.DxfExcelReader import DxfExcelReader
from widgets.CircleTable import CircleTable

import os

class DXFReadThread(QThread):  
    processStep = pyqtSignal(int)  
    def __int__(self):  
        super(DXFReadThread,self).__init__()
        
  
    def setDxf(self,dxf):
        self.dxf=dxf

    def run(self):  
        self.dxf.processStep.connect(self.__process)
        num=self.dxf.read()


    def __process(self,num):
        self.processStep.emit(num)




class ImportCADDiaolg(QDialog):
    def __init__(self, db, parent=None):
        super(ImportCADDiaolg,self).__init__()
        self.database=db

        if parent is not None:
            self.setWindowIcon(parent.windowIcon())

        self.__initVariable()
        self.__initUI()

    def __initVariable(self):
        self.dxffile=None
        self.workThread=None

        self.canUse=True


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

        ###第一行
        label1=QLabel('基桩所在图层',self)
        self.layerCircle=QLineEdit()

        hbox1=QHBoxLayout()
        hbox1.addWidget(label1)
        hbox1.addWidget(self.layerCircle)
        hbox1.addWidget(QLabel('可选        '))
        # hbox1.addStretch(1)

        ###第二行
        label2=QLabel('名称所在图层',self)
        self.layerText=QLineEdit()

        hbox2=QHBoxLayout()
        hbox2.addWidget(label2)
        hbox2.addWidget(self.layerText)
        hbox2.addWidget(QLabel('可选        '))
        # hbox2.addStretch(1)


        #第三行
        label3=QLabel('基桩的半径值')
        self.radiusValue=QLineEdit()

        hbox3=QHBoxLayout()
        hbox3.addWidget(label3)
        hbox3.addWidget(self.radiusValue)
        hbox3.addWidget(QLabel('可选        '))
        # hbox3.addStretch(1)


        ###第三行
        label4=QLabel('基桩命名格式')
        self.formatName=QLineEdit()

        hbox4=QHBoxLayout()
        hbox4.addWidget(label4)
        hbox4.addWidget(self.formatName)
        hbox4.addWidget(QLabel('可选 (s,d,-)'))
        # hbox4.addStretch(1)

        ###第四行
        label5=QLabel('基桩文件路径')
        self.filePath=QLineEdit()
        self.filePath.setEnabled(False)

        hbox5=QHBoxLayout()
        hbox5.addWidget(label5)
        hbox5.addWidget(self.filePath)
        hbox5.addWidget(QLabel('DXF 或 Excel'))
        # hbox5.addStretch(1)


        #第五行
        self.progress=QProgressBar()
        self.progress.setValue(0)


        #第六行
        openFileButton=QPushButton('选择文件')
        runImportButton=QPushButton('运行导入')
        lookButton=QPushButton('查看数据')

        hbox6=QHBoxLayout()
        hbox6.addStretch(1)
        hbox6.addWidget(openFileButton)
        hbox6.addWidget(runImportButton)
        hbox6.addWidget(lookButton)
        

        openFileButton.clicked.connect(self.openFile)
        runImportButton.clicked.connect(self.importData)
        lookButton.clicked.connect(self.lookTable)

        vbox=QVBoxLayout()
        vbox.addLayout(hbox1)
        vbox.addLayout(hbox2)
        vbox.addLayout(hbox3)
        
        vbox.addLayout(hbox4)
        vbox.addLayout(hbox5)
        vbox.addSpacing(10)
        vbox.addWidget(self.progress)
        vbox.addSpacing(10)
        vbox.addLayout(hbox6)
        vbox.addSpacing(10)

        self.setLayout(vbox)


        
        self.setWindowTitle("导入CAD数据")
        self.setWindowFlags(Qt.WindowCloseButtonHint)
        
        self.resize(500,300)
        # self.exec_()

        

    #选择文件路径
    def openFile(self):
        if not self.canUse:
            return

        file, ext = QFileDialog.getOpenFileName(self,  
                                    "文件选择",  
                                    './',  
                                    "Files (*.dxf *.xlsx *.xls)")          
        if len(file)>0:
            self.filePath.setText(file)
            self.dxffile=file


    #导入数据
    def importData(self):
        if self.dxffile is None:
            return

        if not self.canUse:
            return

        self.canUse=False

        self.progress.setMaximum(6)


        if os.path.splitext(self.dxffile)[1].lower()=='.dxf':
            self.__DxfFormat()
        else:
            self.__ExcelFormat()


        self.workThread=DXFReadThread()  
        self.workThread.setDxf(self.dxf)
        self.workThread.processStep.connect(self.__updateProgress)   #当获得完毕的信号时，停止
        # self.workThread.run()              #开始  
        self.workThread.start()




    #dxf格式的数据
    def __DxfFormat(self):
        circleLayer=self.layerCircle.text().strip()
        nameLayer=self.layerText.text().strip()
        raidus=self.radiusValue.text().strip()
        nameFormat=self.formatName.text().strip()

        self.dxf=DxfReader(self.dxffile)

        #设置圆桩层名称
        if len(circleLayer)>0:
            self.dxf.setCircleLayer(circleLayer)

        #设置标号层名称
        if len(nameLayer)>0:
            self.dxf.setNameLayer(nameLayer)

        #设置半径
        try:
            radius=float(radius)
        except:
            radius=None

        if radius is not None:
            self.dxf.setRadius(radius)

        #设置名称格式
        if len(nameFormat)>0:
            self.dxf.setNameFormat(nameFormat)



    #excel格式的数据
    def __ExcelFormat(self):
        self.dxf=DxfExcelReader()
        self.dxf.setExcelFile(self.dxffile)

            


    def __updateProgress(self,num):
        self.progress.setValue(num)

        if num==6:
            names,points,radius=self.dxf.getCircles()
            self.database.insertCircles(names,points,radius)
            self.canUse=True


            

    #查看导入结果
    def lookTable(self):
        if not self.canUse:
            return

        ids,names,locations,radius=self.database.getCircles()

        table=CircleTable(self)
        table.setDataBase(self.database)
        table.setContents(ids,names,locations,radius)
        table.exec_()
        



if __name__=='__main__':
    import sys
    from PyQt5.QtWidgets import QApplication

    app = QApplication(sys.argv)
    viewer=ImportCADDiaolg('./')
    viewer.show()
    
    sys.exit(app.exec_())