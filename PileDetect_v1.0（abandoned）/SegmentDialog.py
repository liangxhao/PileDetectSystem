from PyQt5.QtWidgets import QDialog,QPushButton,QHBoxLayout,QDesktopWidget,QVBoxLayout,QProgressBar,\
    QLabel,QSpacerItem

from PyQt5.QtWidgets import QFileDialog
from PyQt5.QtCore import Qt
from SegmentThread import SegmentThread

from ImageView import ImageViewSegment

class SegmentDialog(QDialog):
    def __init__(self,command,imageViewer,parent=None):
        super(SegmentDialog, self).__init__(parent)
        self.command=command

        self.imageViewer=None
        if type(imageViewer) is  ImageViewSegment:
            self.imageViewer=imageViewer

        self.__initVariable()
        self.__initUI()
        

    def __initVariable(self):
        self.thread=None

        self.count=0

        self.totalCount=0


    def __initUI(self):
        startButton=QPushButton('开始')
        startButton.pressed.connect(self.__startRun)
        stopButton=QPushButton('停止')
        stopButton.pressed.connect(self.__stopRun)

        hbox1=QHBoxLayout()
        hbox1.addWidget(startButton)
        hbox1.addStretch(1)
        hbox1.addWidget(stopButton)

        margin=20
        vbox = QVBoxLayout()
        vbox.setContentsMargins(margin,margin,margin,margin)

        vbox.addLayout(hbox1)

        self.textLabel=QLabel('')
        self.progress=QProgressBar()

        self.progress.setValue(0)
  
        vbox.addSpacing(margin)
        vbox.addWidget(self.textLabel)
        vbox.addSpacing(margin/3)
        vbox.addWidget(self.progress)
        vbox.addSpacing(margin)

        vbox.addStretch(1)
        
        self.setLayout(vbox)

        self.resize(500,220)
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

        self.exec_()



    def __startRun(self):
        if self.thread is not None:
            return


        self.thread = SegmentThread(self.command, self)
        self.thread.SignalResult.connect(
            self.updateShow, type=Qt.QueuedConnection)

        self.thread.SignalNames.connect(
            self.setNamesForImageView, type=Qt.QueuedConnection)
        
        self.thread.start()

        if self.imageViewer is not None:
            self.imageViewer.hideAllItem()
        # self.thread.run()

    def __stopRun(self):
        if self.thread is None:
            return

        self.thread.stop()
        self.thread.terminate()
        del(self.thread)

        self.thread=None
        self.count=0


    def closeEvent(self,event):
        if self.thread is not None:
                self.thread.stop()

        super().closeEvent(event)


    def setNamesForImageView(self,names):
        if self.imageViewer is not None:
            self.imageViewer.setNames(names)


    def updateShow(self,info):
        if 'num' in info:
            self.totalCount=int(info.split(':')[-1])
            self.progress.setMinimum(0)
            self.progress.setMaximum(self.totalCount)
        elif 'OK' in info:
            self.textLabel.setText('完成')
        else:
            self.count=self.count+1
            self.textLabel.setText('第'+str(self.count)+'个')
            self.progress.setValue(self.count)

            if self.imageViewer is not None:
                item=info.split(',')
                if '' not in item and len(item)==5:
                    index=int(item[0])
                    pointsXY=[int(item[1]),int(item[2])]
                    radius=int(item[3])

                    self.imageViewer.addCircle(index,pointsXY,radius)




if __name__=='__main__':
    import sys
    from PyQt5.QtWidgets import QApplication

    app = QApplication(sys.argv)
    viewer=SegmentDialog(['']*5)
    viewer.show()
    
    sys.exit(app.exec_())