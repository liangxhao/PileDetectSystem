from random import uniform
import xlwt
import copy
import os

class ReportFile(object):
    def __init__(self,reportFile):
        self.__reportFile=reportFile


    def generateReportByReadFile(self,originalFile,segmentFile):
        #先读取dxf导出的圆坐标
        pass

    def generateReportByData(self,names,oriXY,oriR,index,segXY,segR,segFlag):
        num=len(names)
        with open(self.__reportFile,'w') as f:
            f.write(str(num)+'\n')

            for i in range(num):
                try:
                    pos=index.index(i)
                    f.write(str(names[i])+','+str(oriXY[i][0])+','+str(oriXY[i][1])+','+str(oriR[i])+\
                                         ','+str(segXY[pos][0])+','+str(segXY[pos][1])+','+str(segR[pos])+\
                                         ','+str(oriXY[i][0]-segXY[pos][0])+','+str(oriXY[i][1]-segXY[pos][1])+\
                                         ','+str(segFlag[pos])+'\n')

                except:
                    f.write(str(names[i])+','+str(oriXY[i][0])+','+str(oriXY[i][1])+','+str(oriR[i])+'\n')
                                         


    def exportTable(self,fileName):
        if not os.path.exists(self.__reportFile):
            return
        
        with open(self.__reportFile,'r') as f:
            content=f.read()
            content=content.splitlines()[1:]


        file=xlwt.Workbook(encoding='utf-8')
        table=file.add_sheet('Sheet 1')

        font=xlwt.Font()
        font.height=220
        font.name= '等线'
        font.bold=True

        style0=xlwt.XFStyle()
        style0.font=font
        style0.alignment.horz=xlwt.Alignment.HORZ_CENTER

        #表头
        table.write(0,0,'Name',style0)
        table.write(0,1,'X(mm)',style0)
        table.write(0,2,'Y(mm)',style0)
        table.write(0,3,'Radius(mm)',style0)
        table.write(0,4,'segX(mm)',style0)
        table.write(0,5,'segY(mm)',style0)
        table.write(0,6,'segRadius(mm)',style0)
        table.write(0,7,'dx(mm)',style0)
        table.write(0,8,'dy(mm)',style0)
        table.write(0,9,'flag',style0)

        style1=copy.deepcopy(style0)
        style1.font.bold=False

        for i in range(len(content)):
            item=content[i].split(',')
            
            
            table.write(i+1,0,item[0],style1)
            table.write(i+1,1,item[1],style1)
            table.write(i+1,2,item[2],style1)
            table.write(i+1,3,item[3],style1)

            if len(item)==10:
                table.write(i+1,4,item[4],style1)
                table.write(i+1,5,item[5],style1)
                table.write(i+1,6,item[6],style1)
                table.write(i+1,7,item[7],style1)
                table.write(i+1,8,item[8],style1)
                table.write(i+1,9,item[9],style1)

        file.save(fileName)