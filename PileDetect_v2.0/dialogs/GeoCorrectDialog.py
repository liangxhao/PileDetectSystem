from PyQt5.QtWidgets import QDialog,QHBoxLayout,QVBoxLayout,QPushButton,QLabel,QLineEdit,QRadioButton
from PyQt5.QtCore import Qt

import os

class GeoCorrectDialog(QDialog):
    def __init__(self, mode=0, parent= None):
        super(GeoCorrectDialog,self).__init__()
        if parent is not None:
            self.setWindowIcon(parent.windowIcon())
        self.mode=mode

        self.__initUI()
        self.resize(300,180)
        self.exec_()



    def __initUI(self):
        self.radio0=QRadioButton("无几何校正（至少要3个控制点）", self)
        self.radio1 = QRadioButton("1阶多项式几何校正（至少要4个控制点）", self)
        self.radio2 = QRadioButton("2阶多项式几何校正（至少要7个控制点）", self)

        if self.mode==0:
            self.radio0.setChecked(True)
        elif self.mode==1:
            self.radio1.setChecked(True)
        elif self.mode==2:
            self.radio2.setChecked(True)


        self.buttonOK=QPushButton('确定',self)
        self.buttonCancel=QPushButton('取消',self)


        vbox1=QVBoxLayout()
        vbox1.addWidget(self.radio0)
        vbox1.addWidget(self.radio1)
        vbox1.addWidget(self.radio2)

        hbox1=QHBoxLayout()
        hbox1.addWidget(self.buttonOK)
        hbox1.addSpacing(50)
        hbox1.addWidget(self.buttonCancel)

        vboxAll=QVBoxLayout()
        vboxAll.addLayout(vbox1)
        vboxAll.addLayout(hbox1)

        self.setLayout(vboxAll)


        self.buttonOK.clicked.connect(self.__Ok)
        self.buttonCancel.clicked.connect(self.__Cancel)


    # 获取模式
    def getMode(self):
        return self.mode


    def __Ok(self):
        if self.radio0.isChecked():
            self.mode=0
        elif self.radio1.isChecked():
            self.mode=1
        elif self.radio2.isChecked():
            self.mode=2

        self.close()

    def __Cancel(self):
        self.close()


if __name__ == "__main__":
    import sys
    from PyQt5.QtWidgets import QApplication
    app = QApplication(sys.argv)
    qb = GeoCorrectDialog()
    qb.show()
    sys.exit(app.exec_())
