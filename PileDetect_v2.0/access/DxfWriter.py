from .DataBase import DataBase
import ezdxf
import os

class DxfWriter(object):
    def __init__(self):
        super(DxfWriter,self).__init__()
        self.__initVariable()


    def __initVariable(self):
        self.names=None
        self.points=None
        self.point_r=None
        self.gPoints=None
        self.gPonits_r=None


        #输出的dxf文件名
        self.outFile=None

    def setImageInfo(self,imageName,size,limit):
        #图像名称
        self.imageName=imageName
        #图像尺寸
        self.size=size
        #图像范围
        self.limit=limit


    def setData(self,dbFile,outFile):
        database=DataBase(dbFile)

        database.connectDB()
        self.names, self.points, self.point_r, self.gPoints, self.gPonits_r, _=database.getReport()

        database.closeDB()
        del database

        self.outFile=outFile


        self.__writer()


    def __writer(self):
        dwg=ezdxf.new('AC1024')

        #图层名称
        old_layer='old'
        new_layer='new'
        name_layer='name'

        if self.point_r:
            height=self.point_r[0]/5
        else:
            height=0

        old_dxfattribs={'layer': old_layer}
        new_dxfattribs={'layer': new_layer}
        name_dxfattribs={'layer': name_layer,'height': height }


        dwg.layers.new(name=old_layer,dxfattribs={'color': 170})
        dwg.layers.new(name=new_layer,dxfattribs={'color': 10})
        dwg.layers.new(name=name_layer,dxfattribs={'color': 80})

        dwg.layers.remove('Defpoints')
        dwg.layers.remove('View Port')

        msp=dwg.modelspace()

        #插入图像
        fileName=os.path.basename(self.imageName)
        my_image_def=dwg.add_image_def(filename='./'+fileName,size_in_pixel=self.size)

        msp.add_image(insert=(self.limit[0],self.limit[2]),size_in_units=(self.limit[1]-self.limit[0],self.limit[3]-self.limit[2]), image_def=my_image_def, rotation=0)


        #cad上的圆
        for point,r in zip(self.points,self.point_r):
            msp.add_circle(point,r,dxfattribs=old_dxfattribs)

        #绘制的圆
        for point,r in zip(self.gPoints,self.gPonits_r):
            if None not in point:
                msp.add_circle(point,r,dxfattribs=new_dxfattribs)

        #文字
        for text,point in zip(self.names,self.points):
            msp.add_text(text,dxfattribs=name_dxfattribs).set_pos(point, align='MIDDLE_RIGHT')


        dwg.saveas(self.outFile)
