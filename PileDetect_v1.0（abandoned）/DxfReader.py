import ezdxf
import numpy as np
from pykdtree.kdtree import KDTree
from CircleObjFile import CircleObjFile

class DxfReader:

    def __init__(self,inputfile,outputfile):
        self.input=inputfile
        self.output=outputfile

        self.queryCircle='CIRCLE'
        self.queryMtext='MTEXT'
    
    def setCircleLayer(self,layer):
        self.queryCircle='CIRCLE[layer=="'+layer+'"]'

    def setMtextLayer(self,layer):
        self.queryMtext='MTEXT[layer=="'+layer+'"]'


    def read(self):
        dxf = ezdxf.readfile(self.input)

        modelspace = dxf.modelspace()
        location=[]
        radiusLoc=[]

        labels=[]
        labelsLoc=[]

        circle = modelspace.query(self.queryCircle)
        for item in circle:
            (x,y,z)=item.dxf.center
            radius=item.dxf.radius

            location.append([x,y])
            radiusLoc.append(radius)


        text = modelspace.query(self.queryMtext)
        for item in text:
            content=item.get_text().split(';')[1][:-1]
            if '-' in content:
                (x,y,z)=item.dxf.insert
                labels.append(content)
                labelsLoc.append([x,y])


        if len(location) == len(labels):
            #构建kdtree
            data_pts_real = np.array(location)
            kdtree = KDTree(data_pts_real)
            query_pts=np.array(labelsLoc)
            dist, idx = kdtree.query(query_pts)

            location=[location[i] for i in idx]
            radiusLoc=[radiusLoc[i] for i in idx]
            
            cof=CircleObjFile(self.output)
            cof.setObjCircleByGround(labels,location,radiusLoc)

            return len(location)

        else:
            return 0