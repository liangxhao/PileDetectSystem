from PyQt5.QtWidgets import QWidget,QListWidget,QVBoxLayout,QHBoxLayout,QLabel,QListWidgetItem,\
    QListView,QFileIconProvider
from PyQt5.QtGui import QPixmap,QIcon
from PyQt5.QtCore import  pyqtSignal,Qt,QSize,QFileInfo
import os

class FileListView(QWidget):

    itemDoubleClicked=pyqtSignal(str)


    def __init__(self):
        super().__init__()
        self.__initUI()

        self.filePath=None



    def __storeFilePath(self,file):
        self.filepath=os.path.split(file)[0]#获取图片所在的目录


    def __initUI(self):

        vbox=QVBoxLayout()

        self.listfiles=QListWidget()

        self.listfiles.setResizeMode(QListView.Adjust)
        self.listfiles.setMovement(QListView.Static)

        vbox.addWidget(self.listfiles)
        
        self.setLayout(vbox)


        self.listfiles.doubleClicked.connect(self.listClicked)
        
        
    def listClicked(self,index):
        item =self.listfiles.currentItem().text()

        file=self.filepath+'/'+item
        # print(item.text())
        self.itemDoubleClicked.emit(file)
        

    def insertFiles(self,files):
        self.listfiles.clear()

        if len(files)==0:
            return

        self.__storeFilePath(files[0])

        fileInfo = QFileInfo(files[0])
        fileIcon = QFileIconProvider()
        icon = QIcon(fileIcon.icon(fileInfo))#获取文件图标

        for index,item in enumerate(files):

            name=os.path.basename(item)
            value=QListWidgetItem(icon,name)

            self.listfiles.insertItem(index,value)



    def clearFiles(self):
        self.listfiles.clear()

