import xlrd

from PyQt5.QtCore import QObject,pyqtSignal

#读取Excel格式的cad数据
class DxfExcelReader(QObject):
    processStep=pyqtSignal(int)

    def __init__(self):
        super(DxfExcelReader,self).__init__()
        self.__initVariable()

    

    def __initVariable(self):
        self.names=[]
        self.points=[]
        self.radius=[]

        self.excelFile=None


    def setExcelFile(self,file):
        self.excelFile=file



    def read(self):

        data=xlrd.open_workbook(self.excelFile)
        table=data.sheet_by_index(0)
        ncols=table.ncols
        nrows=table.nrows

        #列数必须为3
        if ncols !=4:
            return      
        #至少有1行
        if nrows<1:
            return

        for i in range(nrows):
            row=table.row_values(i)
            point=[0,0]
            

            try:
                name=str(row[0])
                point[0]=float(row[1])
                point[1]=float(row[2])
                r=float(row[3])
            except:
                continue
            
            self.names.append(name)
            self.points.append(point)
            self.radius.append(r)


        self.processStep.emit(6)
        return len(self.names)


    def getCircles(self):
        return self.names,self.points,self.radius

