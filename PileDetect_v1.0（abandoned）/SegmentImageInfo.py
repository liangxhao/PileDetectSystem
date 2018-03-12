import os
class SegmentImageInfo(object):
    def __init__(self,file):
        self.__infoFile=file

    def getImageFilePath(self):
        if not os.path.exists(self.__infoFile):
            return ''
        with open(self.__infoFile,'r') as f:
            path=f.readline().replace('\n','')

        return path


    def setImageFilePath(self,path):
        with open(self.__infoFile,'w') as f:
            f.write(path)


