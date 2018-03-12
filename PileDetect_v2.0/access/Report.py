import xlwt
import copy
import os

class Report(object):
    def __init__(self,outfile):
        self.outfile=outfile


    def setContents(self,names,points,point_r,gPoints,gPoints_r,errors):
        self.names=names
        self.points=points
        self.points_r=point_r
        self.gPoints=gPoints
        self.gPoints_r=gPoints_r
        self.errors=errors

        self.__export()


                                     

    def __export(self):
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
        table.write(0,1,'X_CAD',style0)
        table.write(0,2,'Y_CAD',style0)
        table.write(0,3,'R_CAD',style0)
        table.write(0,4,'X_Image',style0)
        table.write(0,5,'Y_Image',style0)
        table.write(0,6,'R_Image',style0)
        table.write(0,7,'dx',style0)
        table.write(0,8,'dy',style0)
        table.write(0,9,'dr',style0)


        style1=copy.deepcopy(style0)
        style1.font.bold=False

        for i,(name,point,r,gPoint,gr,error) in enumerate(zip(self.names,self.points,self.points_r,self.gPoints,self.gPoints_r,self.errors)):
            (x,y)=point
            (gx,gy)=gPoint     
            (ex,ey,er)=error

            table.write(i+1,0,name,style1)
            table.write(i+1,1,x,style1)
            table.write(i+1,2,y,style1)
            table.write(i + 1, 3, r, style1)

            if gx is not None:
                table.write(i+1,4,gx,style1)
                table.write(i+1,5,gy,style1)
                table.write(i + 1, 6, gr, style1)

            if ex is not None:
                table.write(i+1,7,ex,style1)
                table.write(i+1,8,ey,style1)
                table.write(i + 1, 9, er, style1)

        file.save(self.outfile)