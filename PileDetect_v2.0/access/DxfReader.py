import ezdxf
import numpy as np
import re
from pykdtree.kdtree import KDTree
from PyQt5.QtCore import QObject,pyqtSignal


class DxfReader(QObject):

    processStep=pyqtSignal(int)

    def __init__(self,dxffile):
        super(DxfReader,self).__init__()
        self.dxffile=dxffile       
        self.__initVariable()


    def __initVariable(self):
        self.names=[]
        self.points=[]
        self.radius=[]

        self.queryCircle='CIRCLE'  #查询圆
        self.queryMtext='MTEXT'    #查询多行文字
        self.queryText='TEXT'      #查询文字

        self.queryInsert='INSERT'  #查询跨参照

        self.nameLayer=None  #文字所在图层
        self.circleLayer=None #圆所在图层
        self.constRadius=None #圆的半径
        self.regex=None #名称的格式


    #设置圆所在的图层
    def setCircleLayer(self,layer):
        layer=layer.split(',')
        self.circleLayer=layer

    #设置名称所在的图层
    def setNameLayer(self,layer):
        layer=layer.split(',')
        self.nameLayer=layer

    #设置圆的半径
    def setRadius(self,r):
        self.constRadius=r


    #设置字符串的格式
    def setNameFormat(self,flag):
        flag=str(flag).replace(' ','')
        flagSet=['s','d','-']
        for item in flag:
            if item not in flagSet:
                return

        d='\d+'
        s='[^0-9-]+'
        c='-'

        regex=''
        for item in flag:
            #字符
            if item == flagSet[0]:
                regex+=s
            #数字
            if item == flagSet[1]:
                regex+=d

            #连接符
            if item == flagSet[2]:
                regex+=c

        regex+='$'

        self.regex=regex

        
        


    def read(self):
        #读取数据
        self.processStep.emit(1)

        dxf = ezdxf.readfile(self.dxffile)

        modelspace = dxf.modelspace()

        gPoints=[]
        gRadius=[]

        gNames=[]
        gNamePoints=[]

        #读取圆
        self.processStep.emit(2)
        #查询圆
        quCircles=[]
        if self.circleLayer is not None:
            for layer in self.circleLayer:
                s=self.queryCircle+'[layer=="'+layer+'"]'
                quCircles.append(s)
        else:
            quCircles.append(self.queryCircle)

        for quCircle in quCircles:
            circle = modelspace.query(quCircle)
            for item in circle:
                (x,y,_)=item.dxf.center
                r=item.dxf.radius

                gPoints.append([x,y])
                gRadius.append(r)

        #################################################################
        #读取文字
        self.processStep.emit(3)
        #查询文字
        quTexts=[]
        if self.nameLayer is not None:
            for layer in self.nameLayer:
                s=self.queryText+'[layer=="'+layer+'"]'
                quTexts.append(s)
        else:
            quTexts.append(self.queryText)

        for quText in quTexts:
            text = modelspace.query(quText)
            for item in text:
                content=item.dxf.text
                (x,y,_)=item.dxf.insert

                gNames.append(content)
                gNamePoints.append([x,y])

        #################################################################
        #读取多行文字
        self.processStep.emit(4)
        #查询多行文字
        quMtexts=[]
        if self.nameLayer is not None:
            for layer in self.nameLayer:
                s=self.queryMtext+'[layer=="'+layer+'"]'
                quMtexts.append(s)
        else:
            quMtexts.append(self.queryMtext)

        for quMtext in quMtexts:
            text = modelspace.query(quMtext)
            for item in text:
                content=item.get_text()
                content=content.lstrip('{').rstrip('}').split('\P')
                if len(content)>1:
                    continue
                content=content[-1].split(';')[-1]

                (x,y,_)=item.dxf.insert

                gNames.append(content)
                gNamePoints.append([x,y])

        

        gPoints=np.array(gPoints)
        gRadius=np.array(gRadius)
        #匹配数据
        self.processStep.emit(5)

        if len(gPoints)==0 or len(gNames)==0:
            self.processStep.emit(6)
            return 0

        #如果没有设备圆的半径，就取众数作为默认值
        if self.constRadius is None:
            rs=np.unique(gRadius).tolist()
            gr=gRadius.tolist()
            num=[gr.count(x) for x in rs]
            self.constRadius=rs[num.index(max(num))]

        #去掉半径长度不符合的圆
        # change
        # index=np.where(gRadius==self.constRadius)
        # gPoints=gPoints[index]
        # gRadius=gRadius[index]



        gNames=np.array(gNames)
        gNamePoints=np.array(gNamePoints)

        #去掉不匹配的名称
        if self.regex is not None:
            pattern = re.compile(self.regex)
            index=[pattern.match(x) is not None for  x in gNames]

            index=np.array(index)

            gNames=gNames[index]
            gNamePoints=gNamePoints[index]


        #圆与名称之间的匹配
        distThreshold=18*self.constRadius

        ##逐个名称取匹配每个圆，找到距离最近的圆
        kdtree = KDTree(gPoints)  
        dist, idx = kdtree.query(gNamePoints)

        index=np.where(dist<distThreshold)
         
        gNames=gNames[index].tolist()
        idx=idx[index]

        gPoints=gPoints[idx].tolist()
        gRadius=gRadius[idx].tolist()

        ##逐个圆匹配每个名称，找到最近的名称
        # kdtree = KDTree(gNamePoints)
        # dist,idx = kdtree.query(gPoints)

        # index=np.where(dist<distThreshold)
        # gPoints=gPoints[index].tolist()
        # gRadius=gRadius[index].tolist()

        # idx=idx[index]
        # gNames=gNames[idx].tolist()


        self.names=gNames
        self.points=gPoints
        self.radius=gRadius

        self.processStep.emit(6)
        return len(self.names)


    def getCircles(self):
        return self.names,self.points,self.radius




if __name__=='__main__':
    # file='G:/Python/DXFData/object.dxf'
    file='C:/Users/liang/Desktop/test/data/object.dxf'
    dxf=DxfReader(file)

    # dxf.setNameLayer('PUB_TEXT')
    # dxf.setCircleLayer('抗压兼抗拔桩,抗压桩')
    # dxf.setNameFormat('d')
    dxf.setNameLayer('0')
    dxf.setCircleLayer('ZH')
    dxf.setNameFormat('d-d')

    dxf.read()

    print(dxf.getCircles())
