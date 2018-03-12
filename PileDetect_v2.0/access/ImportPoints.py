import xlrd
import types
import os
import re


class ImportPoints(object):
    def __init__(self,file):
        self.file=file

        self.__initVariable()
        
        if os.path.splitext(self.file)[1].lower()=='.txt':
            self.__txtread()
        else:
            self.__xlread()
        


    def __initVariable(self):
        self.names=[]
        self.points=[]


    def __xlread(self):
        self.points.clear()

        data=xlrd.open_workbook(self.file)
        table=data.sheet_by_index(0)
        ncols=table.ncols
        nrows=table.nrows

        #列数必须为3
        if ncols <3:
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
            except:
                continue
            
            self.names.append(name)
            self.points.append(point)



    def __txtread(self):
        with open(self.file) as f:
            for line in f:
                line=line.replace('\n','')
                row=re.split(',|，|\s|\t',line)#分割字符串
                point=[0,0]

                if len(row)!=3:
                    continue
                
                try:
                    name=str(row[0])
                    point[0]=float(row[1])
                    point[1]=float(row[2])
                except:
                    continue

                self.names.append(name)
                self.points.append(point)



    def getPoints(self):
        return self.names,self.points


            

            
                



        



if __name__=='__main__':
    p=ImportPoints('C:/Users/liangxhao/Desktop/tmp/1.xlsx','1.txt')
    