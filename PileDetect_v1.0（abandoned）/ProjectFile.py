class ProjectFile(object):
    def __init__(self,file):

        self.__projectFile=file

    def createProject(self):
        with open(self.__projectFile,"w") as f:
            f.write('0\n')
            f.write('0\n')
        

    def setNoramlImages(self,filesName):
        with open(self.__projectFile,"r") as f:
            contens=f.readlines()

            num=int(contens[0].replace('\n',''))
            contens=contens[num+1:]

        #写入工程文件
        with open(self.__projectFile,"w") as f:
            #图片数量
            f.write(str(len(filesName))+'\n')
            #图片文件名
            for item in filesName:
                f.write(str(item)+'\n')

            f.writelines(contens)


    def setFullImages(self,filesName):
        with open(self.__projectFile,"r") as f:
            contens=f.readlines()
            #第一部分
            num1=int(contens[0].replace('\n',''))      
            contens1=contens[0:num1+1]
            #第二部分
            num2=int(contens[num1+1].replace('\n',''))
            contens2=contens[num1+num2+2:]


        #写入工程文件
        with open(self.__projectFile,"w") as f:
            f.writelines(contens1)
            #图片数量
            f.write(str(len(filesName))+'\n')
            #图片文件名
            for item in filesName:
                f.write(str(item)+'\n')

            f.writelines(contens2)


    def getImages(self):
        with open(self.__projectFile,"r") as f:
            contens=f.readlines()

            #第一部分
            num1=int(contens[0].replace('\n',''))      
            contens1=contens[1:num1+1]

            #第二部分
            num2=int(contens[num1+1].replace('\n',''))
            contens2=contens[num1+2:num1+num2+2]

            contens1=[item.replace('\n','')  for item in contens1]
            contens2=[item.replace('\n','')  for item in contens2]

            return contens1,contens2


