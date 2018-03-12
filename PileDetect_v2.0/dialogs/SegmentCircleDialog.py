from PyQt5.QtWidgets import QDialog,QPushButton,QHBoxLayout,QDesktopWidget,QVBoxLayout,QProgressBar,\
    QLabel,QSpacerItem

from PyQt5.QtWidgets import QFileDialog
from PyQt5.QtCore import Qt,pyqtSignal
from PyQt5.QtGui import QIcon
from functions.SegmentThread import SegmentThread
import icon

class SegmentCircleDialog(QDialog):
    # 发送分割结果(id,x,y,r)
    segmentResultSignal=pyqtSignal(int,int,int,int)
    # 发送结束分割的信号
    endSegmentSignal=pyqtSignal()

    def __init__(self,parent=None):
        super(SegmentCircleDialog, self).__init__(parent)
        self.setWindowIcon(QIcon(':/image/program.png'))

        self.__initVariable()
        self.__initUI()


    def __initVariable(self):
        self.thread = None

        self.count = 0

        self.totalCount = 0


    def __initUI(self):
        startButton = QPushButton('开始')
        startButton.pressed.connect(self.__startRun)
        stopButton = QPushButton('停止')
        stopButton.pressed.connect(self.__stopRun)

        hbox1 = QHBoxLayout()
        hbox1.addWidget(startButton)
        hbox1.addStretch(1)
        hbox1.addWidget(stopButton)

        margin = 20
        vbox = QVBoxLayout()
        vbox.setContentsMargins(margin, margin, margin, margin)

        vbox.addLayout(hbox1)

        self.textLabel = QLabel('')
        self.progress = QProgressBar()

        self.progress.setValue(0)

        vbox.addSpacing(margin)
        vbox.addWidget(self.textLabel)
        vbox.addSpacing(margin / 3)
        vbox.addWidget(self.progress)
        vbox.addSpacing(margin)

        vbox.addStretch(1)

        self.setLayout(vbox)

        self.resize(500, 200)
        self.setWindowFlags(Qt.WindowCloseButtonHint)
        self.setWindowTitle('图像分割')

        self.setStyleSheet("""
                    QPushButton{
    
                        width: 80px;  
                        height:20px;  
    
                    }
                    QLabel{
                        font-size:14px;
                    }
                """)

        # self.exec_()
    def setSegmentInfo(self, image_name,all_circles, needSegment_Idx, segmentProgram):
        self.image_name = image_name
        self.all_circles = all_circles
        self.needSegment_idx = needSegment_Idx
        self.segmentProgram=segmentProgram



    def __startRun(self):
        if self.thread is not None:
            return

        self.thread = SegmentThread()
        self.thread.setInputSegmentInfo(self.image_name,self.all_circles, self.needSegment_idx,self.segmentProgram)
        self.thread.segmentThreadResult.connect(self.updateShow)

        self.thread.start()



    def __stopRun(self):
        if self.thread is None:
            return

        self.thread.stop()
        self.thread.terminate()
        del self.thread

        self.thread = None
        self.count = 0



    def closeEvent(self,event):
        if self.thread is not None:
                self.thread.stop()

        super().closeEvent(event)


    def updateShow(self,info):
        if 'num' in info:
            self.totalCount=int(info.split(':')[-1])
            self.progress.setMinimum(0)
            self.progress.setMaximum(self.totalCount)
        elif 'OK' in info:
            self.textLabel.setText('完成')
            self.endSegmentSignal.emit()
        else:
            self.count=self.count+1
            self.textLabel.setText('第'+str(self.count)+'个')
            self.progress.setValue(self.count)

            item = info.split(',')
            if '' not in item and len(item) == 5:
                index = int(item[0])
                px=int(item[1])
                py=int(item[2])
                pr=int(item[3])

                self.segmentResultSignal.emit(index,px,py,pr)






if __name__ == '__main__':
    import sys
    from PyQt5.QtWidgets import QApplication

    app = QApplication(sys.argv)
    viewer = SegmentCircleDialog()
    viewer.show()

    sys.exit(app.exec_())