import codecs
import locale
import os
from subprocess import Popen
import subprocess
from Attribute import Attribute

from PyQt5.QtCore import QThread, pyqtSignal, Qt
from PyQt5.QtWidgets import QPlainTextEdit, QApplication, QVBoxLayout,\
    QDialog,QPushButton,QHBoxLayout,QDesktopWidget



class Thread(QThread):

    SignalAppendText = pyqtSignal(str)
    SignalExit = pyqtSignal()

    def __init__(self, command, *args, **kwargs):
        super(Thread, self).__init__(*args, **kwargs)
        self.command = command
        self.running = 1
        
        self.stopCommand='taskkill /F /IM stitching.exe'
        
    def stop(self):
        self.running = 0
        if hasattr(self, "process"):
            self.process.kill()
            os.system(self.stopCommand)#调用外部命令结束
        # print("thread stoped")

    def run(self):
        self.process = Popen(self.command, startupinfo=Attribute.startupinfo,stdout=subprocess.PIPE, shell=True)
        while self.running:
            data = self.process.stdout.readline()
            if not data:
                self.SignalExit.emit()
                break
            else:
                data = data.decode(
                    codecs.lookup(locale.getpreferredencoding()).name)
                self.SignalAppendText.emit(data)


class StitchingDialog(QDialog):

    def __init__(self,command):
        super(StitchingDialog, self).__init__()
        self.thread = None
        self.command=command
        self.__initUI()


    def __initUI(self):
        self.startButton=QPushButton('开始')
        self.startButton.pressed.connect(self.startRun)
        self.stopButton=QPushButton('停止')
        self.stopButton.pressed.connect(self.stopRun)

        hbox=QHBoxLayout()
        hbox.addWidget(self.startButton)
        hbox.addStretch(1)
        hbox.addWidget(self.stopButton)

        vbox = QVBoxLayout()
        vbox.addLayout(hbox)

        self.resultEdit = QPlainTextEdit(self, readOnly=True)
        vbox.addWidget(self.resultEdit)
        self.setLayout(vbox)
        self.setWindowTitle('图像拼接')
        self.setWindowFlags(Qt.WindowCloseButtonHint)
        self.setStyleSheet("""
            QPlainTextEdit{
                font-family: Microsoft YaHei;
                background: #CEFDE1;
                
            }
        """)
        screen=QDesktopWidget().screenGeometry(self)
        
        self.resize(int(screen.width()/2),int(screen.height()*2/3))

        self.exec_()
        

    def startRun(self):
        if self.thread:
            return
        self.thread = Thread(self.command, self)
        self.thread.SignalAppendText.connect(
            self.onTextAppend, type=Qt.QueuedConnection)
        self.thread.SignalExit.connect(self.onExit, type=Qt.QueuedConnection)
        self.thread.start()

    def stopRun(self):
        if self.thread:
            self.thread.stop()
            self.thread = None


    def onExit(self):
        self.thread = None

    def onTextAppend(self, text):
        self.resultEdit.appendPlainText(text)

    def closeEvent(self, event):
        if self.thread:
            self.thread.stop()
        super(StitchingDialog, self).closeEvent(event)




if __name__ == "__main__":
    import sys
    app = QApplication(sys.argv)
    ss='G:/Python/Project/Program/modules/stitching.exe C:/Users/liangxhao/Desktop/1/stitiching.pro'
    w = StitchingDialog(ss)
        
    sys.exit(app.exec_())
