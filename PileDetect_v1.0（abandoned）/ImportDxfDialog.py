from PyQt5.QtWidgets import QFileDialog,QGridLayout,QLabel,QPushButton,QLineEdit,QMessageBox,QDialog 
from PyQt5.QtCore import Qt,QThread,pyqtSignal
from DxfReader import DxfReader
from DxfTable import DxfTable
import os

class DXFReadThread(QThread):  
    trigger = pyqtSignal(int)  
    def __int__(self):  
        super(WorkThread,self).__init__()  
        
  
    def setDxf(self,dxf):
        self.dxf=dxf

    def run(self):  
        num=self.dxf.read()
        self.trigger.emit(num)         #完毕后发出信号  


class ImportDxfDiaolg(QDialog):
    def __init__(self, fileName, parent=None):
        super(ImportDxfDiaolg,self).__init__(parent)
        self.fileName=fileName

        self.__initVariable()
        self.__initUI()

    def __initVariable(self):
        self.input=None


    def __initUI(self):
        grid=QGridLayout()
        grid.setSpacing(13)
        grid.setContentsMargins(40,40,40,40)
        grid.setAlignment(Qt.AlignCenter)
        grid.setHorizontalSpacing(25)

        self.setStyleSheet("*{font-size:12px;}")

        ###第一行
        label1=QLabel('圆桩所在图层',self)

        self.layerCircle=QLineEdit()
        

        grid.addWidget(label1,1,0)
        grid.addWidget(self.layerCircle,1,1,1,2)


        ###第二行
        label2=QLabel('标号所在图层',self)
        self.layerText=QLineEdit()

        grid.addWidget(label2,2,0)
        grid.addWidget(self.layerText,2,1,1,2)


        ###第三行
        buttonSelect=QPushButton('选择文件')
        buttonSelect.clicked.connect(self.openFile)

        self.labelPath=QLabel(' ',self)
        self.labelRuning=QLabel(' ',self)


        grid.addWidget(buttonSelect,3,0)
        grid.addWidget(self.labelPath,3,1,1,2)
        grid.addWidget(self.labelRuning,3,3,1,1)


        ###第四行
        buttonOK=QPushButton('导入')
        buttonOK.clicked.connect(self.buttonOKClicked)
        labelHint=QLabel('尽量填写，可留空',self)

        grid.addWidget(labelHint,4,1,1,2)
        grid.addWidget(buttonOK,4,3,1,1)

        buttonLook=QPushButton('查看')
        buttonLook.clicked.connect(self.lookTable)
        grid.addWidget(buttonLook,5,3,1,1)

        self.setLayout(grid)
        self.setWindowTitle("导入CAD数据")
        self.setWindowFlags(Qt.WindowCloseButtonHint)
        
        self.exec_()

        

    #选择文件路径
    def openFile(self):
        file, ext = QFileDialog.getOpenFileName(self,  
                                    "文件选择",  
                                    './',  
                                    "DXF Files (*.dxf)")          
        if len(file)>0:
            self.labelPath.setText(file)
            self.input=file

    #导入数据
    def buttonOKClicked(self):
        if self.input is not None:
            self.labelRuning.setText('正在导入...')

            circleLayer=self.layerCircle.text().strip()
            textlayer=self.layerText.text().strip()

            self.dxf=DxfReader(self.input,self.fileName)
            if len(circleLayer)>0:
                self.dxf.setCircleLayer(circleLayer)#设置圆桩层名称

            if len(textlayer)>0:
                self.dxf.setMtextLayer(textlayer)#设置标号层名称

            # num=dxf.read()
            self.workThread=DXFReadThread()  
            self.workThread.setDxf(self.dxf)
            self.workThread.trigger.connect(self.importCompleted)   #当获得完毕的信号时，停止
            self.workThread.start()              #开始  
            
    #导入完成
    def importCompleted(self,num):
        
        if num>0:
            self.labelRuning.setText('导入完成')
            QMessageBox.information(self,'成功','共导入'+str(num)+'个')

        else:
            self.labelRuning.setText('导入失败')
            QMessageBox.information(self,'失败','无法正确识别圆桩个数')
            

    #查看导入结果
    def lookTable(self):

        if os.path.exists(self.fileName):
            self.dxf=DxfTable(self.fileName)
        else:
            QMessageBox.information(self,'提示','请先导入数据')



if __name__=='__main__':
    import sys
    from PyQt5.QtWidgets import QApplication

    app = QApplication(sys.argv)
    viewer=ImportDxfDiaolg('./')
    
    sys.exit(app.exec_())