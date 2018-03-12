from PyQt5.QtWidgets import QTableWidget,QMainWindow,QTableWidgetItem,QHBoxLayout,QHeaderView,QDialog,\
    QAbstractItemView
from PyQt5.QtCore import Qt
import os

class LocationTable(QDialog):
    def __init__(self, imageName,location,parent=None):
        super(LocationTable,self).__init__(parent)
        self.imageName=imageName
        self.location=location

        self.__initVariable()
        self.__initUI()

    def __initVariable(self):
        self.NameList=[]
        self.Longitude=[]
        self.Latitude=[]


    def __initUI(self):

        self.table=QTableWidget(0,3)
        
        self.table.setHorizontalHeaderLabels(['Name','Longitude','Latitude'])
        self.table.verticalHeader().setVisible(True)
        self.table.horizontalHeader().setSectionResizeMode(QHeaderView.Stretch)
        # self.table.horizontalHeader().setSectionResizeMode(QHeaderView.ResizeToContents)
        self.table.horizontalHeader().setStretchLastSection(True)
        self.table.setHorizontalScrollBarPolicy(Qt.ScrollBarAlwaysOff)
        self.table.setEditTriggers(QAbstractItemView.NoEditTriggers)
        self.table.setSelectionBehavior(QAbstractItemView.SelectRows)
        

        self.__importFile()
        self.__insertLocation()

        hbox = QHBoxLayout()
        hbox.addWidget(self.table)
        self.setLayout(hbox)

        self.setWindowTitle("图像经纬度坐标")
        self.resize(600,500)
        self.exec_()

    def __insertLocation(self):
        if len(self.NameList)!=len(self.Latitude):
            return
        
        self.table.setRowCount(len(self.NameList))

        if len(self.NameList)>0:
            self.table.horizontalHeader().setSectionResizeMode(QHeaderView.ResizeToContents)
        
        for i,(name,lon,lat) in enumerate(zip(self.NameList,self.Longitude,self.Latitude)):
            nameItem=QTableWidgetItem(name)
            nameItem.setTextAlignment(Qt.AlignCenter)
            self.table.setItem(i,0,nameItem)
            #x
            xItem=QTableWidgetItem(lon)
            xItem.setTextAlignment(Qt.AlignCenter)
            self.table.setItem(i,1,xItem)
            #y
            yItem=QTableWidgetItem(lat)
            yItem.setTextAlignment(Qt.AlignCenter)
            self.table.setItem(i,2,yItem)


    def __importFile(self):
        self.NameList.clear()
        self.Longitude.clear()
        self.Latitude.clear()
        try:
            with open(self.imageName,'r') as f:
                line=f.readline().replace('\n','')
                num=int(line)
                for i in range(num):
                    line=f.readline().replace('\n','')
                    item=os.path.basename(line)
                    self.NameList.append(item)

    
            with open(self.location,'r') as f:
                for (index,line) in enumerate(f):
                    if index>0:
                        item=line.replace('\n','').split(',')
                        self.Longitude.append(item[0])
                        self.Latitude.append(item[1])
                        item.clear()
        except:
            return




