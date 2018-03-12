from PyQt5.QtCore import QObject,QThread,pyqtSignal

from access.ImportComposite import ImportComposite


class CompositeThread(QThread):
    process=pyqtSignal(bool)
    def __init__(self):
        super(CompositeThread,self).__init__()
        self.__initVariable()



    def __initVariable(self):
        self.names=None
        self.points=None
        self.points_r=None
        self.gPoints=None
        self.gPoints_r=None
        self.radius=None


    def setFile(self,imageFile,dbFile):
        self.imageFile=imageFile
        self.dbFile=dbFile


    def run(self):
        impo=ImportComposite()

        flag=impo.loadImage(self.imageFile)
        self.image=impo.image

        if not flag:   
            self.process.emit(False)
            return

        self.names, self.points, self.points_r, self.gPoints, self.gPoints_r=impo.loadData(self.dbFile)

        if not self.names:
            self.process.emit(False)
            return
        
        self.process.emit(True)



class LookComposite(QObject):
    def __init__(self,parent):
        super(QObject,self).__init__()
        self.parent=parent


    #显示控件
    def setImageViewer(self,imageViewer):
        self.imageViewer=imageViewer


    def setFile(self,imageFile,dbFile):
        self.comp=CompositeThread()
        self.comp.setFile(imageFile,dbFile)

        self.comp.process.connect(self.__updataView)

        self.comp.start()
        # self.comp.run()


    def __updataView(self,flag):
        if flag == True:
            self.imageViewer.setImage(self.comp.image)
            self.imageViewer.setAlternativeCircles(self.comp.names,self.comp.points,self.comp.points_r)
            self.imageViewer.setSelectedCircles(self.comp.gPoints,self.comp.gPoints_r)
        else:
            if self.comp.image is not None:
                self.imageViewer.setImage(self.comp.image)


        del self.comp
        del self.parent.tmpObject
        self.parent.tmpObject=None

        





