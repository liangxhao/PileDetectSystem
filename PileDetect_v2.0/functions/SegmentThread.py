from PyQt5.QtCore import QThread, pyqtSignal, Qt
from tempfile import NamedTemporaryFile
from access.SegmentInfoWriter import SegmentInfoWriter
import codecs
import locale
import os
from subprocess import Popen
import subprocess

class SegmentThread(QThread):
    segmentThreadResult=pyqtSignal(str)

    def __init__(self):
        super(SegmentThread,self).__init__()
        self.__initVariable()


    def setInputSegmentInfo(self,image_name,all_circles, needSegment_idx,segmentProgram):
        self.image_name=image_name
        self.all_circles=all_circles
        self.needSegment_idx=needSegment_idx
        self.segmentProgram = segmentProgram


    def __initVariable(self):
        self.stopCommand='taskkill /F /IM segment.exe'
        #所调用的外部exe是否正在运行
        self.ExErunning = False


    def __stopExe(self):
        if not self.ExErunning:
            return

        if hasattr(self, "process"):
            self.process.kill()
            os.system(self.stopCommand)#调用外部命令结束

        self.ExErunning = False


    def stop(self):
        self.__stopExe()




    def run(self):
        # 输入到exe，描述圆的文件
        with NamedTemporaryFile('w+t', delete=False) as f:
            segmentInfoFile = f.name.replace("\\", '/')

        # 输出分割结果的文件
        with NamedTemporaryFile('w+t', delete=False) as f:
            segmentResultFile = f.name.replace("\\", '/')

        infoWriter=SegmentInfoWriter(segmentInfoFile)
        infoWriter.setSegmentInfo(self.all_circles, self.needSegment_idx)

        # 图像分割的参数命令
        cmd = []
        cmd.append(self.segmentProgram)
        cmd.append(self.image_name)
        cmd.append(segmentInfoFile)
        cmd.append(segmentResultFile)

        # 子线程的cmd窗口属性
        startupinfo = subprocess.STARTUPINFO()
        startupinfo.dwFlags |= subprocess.STARTF_USESHOWWINDOW
        startupinfo.wShowWindow = subprocess.SW_HIDE

        # 开始进行图像分割
        self.process = Popen(cmd, startupinfo=startupinfo, stdout=subprocess.PIPE, shell=False)

        self.ExErunning=True

        while self.ExErunning:
            data = self.process.stdout.readline()
            if not data:
                # self.SignalExit.emit()
                break
            else:
                data = data.decode(
                    codecs.lookup(locale.getpreferredencoding()).name)
                self.segmentThreadResult.emit(data)
        self.ExErunning=False

        self.segmentThreadResult.emit("OK")




