from PyQt5.QtWidgets import QDialog,QHBoxLayout,QVBoxLayout,QPushButton

from PyQt5.QtCore import Qt  
  
class LookCircleDialog(QDialog):  
    def __init__(self, parent= None):  
        super(LookCircleDialog,self).__init__()
        if parent is not None:
            self.setWindowIcon(parent.windowIcon())

        self.setStyleSheet("*{font-size: 15px;}")
        
        self.setWindowTitle('选择查看方式')  
        self.setWindowFlags(Qt.WindowCloseButtonHint)
          

        button1=QPushButton('表格形式')
        button1.clicked.connect(self.__onButton1)
        button2=QPushButton('视图形式')
        button2.clicked.connect(self.__onButton2)


        hbox=QHBoxLayout()
        hbox.addStretch(1)
        hbox.addWidget(button1)
        hbox.addSpacing(10)
        hbox.addWidget(button2)
        hbox.addStretch(1)

        self.setLayout(hbox)

        self.resize(250,100)

        self.flag=0


    def __onButton1(self):
        self.reject()
        self.flag=1

    def __onButton2(self):
        self.reject()
        self.flag=2
        




  
if __name__ == "__main__":  
    import sys  
    from PyQt5.QtWidgets import QApplication
    app = QApplication(sys.argv)  
    qb = lookCircleDialog()  
    qb.show()  
    sys.exit(app.exec_())