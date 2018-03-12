from PyQt5.QtWidgets import QWidget,QListWidget,QVBoxLayout,QHBoxLayout,QLabel,QListWidgetItem,\
    QListView,QFileIconProvider
from PyQt5.QtGui import QPixmap,QIcon
from PyQt5.QtCore import  pyqtSignal,Qt,QSize,QFileInfo
import os
import icon


class FileListView(QWidget):

    selectImage=pyqtSignal(int,str)


    def __init__(self,parent=None):
        super(FileListView,self).__init__()
        self.__initUI()

        self.filePath=None
        self.ids=None



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
        
        
    def listClicked(self,model):
        row=model.row()

        idx=self.ids[row]
        item =self.listfiles.currentItem().text()

        file=self.filepath+'/'+item
        # print(item.text())
        self.selectImage.emit(idx,file)
        

    def insertFiles(self,ids,files,states):
        self.ids=ids
        self.listfiles.clear()

        if len(files)==0 or len(files)!= len(states):
            return

        self.__storeFilePath(files[0])

        # fileInfo = QFileInfo(files[0])
        # fileIcon = QFileIconProvider()
        # icon = QIcon(fileIcon.icon(fileInfo))#获取文件图标
        icon0 = QIcon(':/image/fileState0.png')
        icon1 = QIcon(':/image/fileState1.png')
        icon2 = QIcon(':/image/fileState2.png')

        icon=[icon0,icon1,icon2]

        height=self.listfiles.height()/48     
        self.listfiles.setIconSize(QSize(height*3,height))
        
        
        for row,value in enumerate(files):

            name=os.path.basename(value)
            item=QListWidgetItem(icon[states[row]],name)

            self.listfiles.insertItem(row,item)


    #更新某一项的图标
    def updateState(self,idx,state):
        #找到idx所在的行
        row=self.ids.index(idx)

        path=':/image/fileState'+str(state)+'.png'
        icon=QIcon(path)
        item=self.listfiles.item(row)
        item.setIcon(icon)

        self.listfiles.insertItem(row,item)

    # 重置所有图像的状态图标
    def resetState(self):
        num=len(self.ids)
        for row in range(num):
            path = ':/image/fileState0.png'
            icon = QIcon(path)
            item = self.listfiles.item(row)
            item.setIcon(icon)

            self.listfiles.insertItem(row, item)




    def clearFiles(self):
        self.ids.clear()
        self.listfiles.clear()

