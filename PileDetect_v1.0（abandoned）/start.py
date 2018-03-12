import sys
from PyQt5.QtWidgets import QApplication
from MainUI import MainUI


if __name__=='__main__':

    app = QApplication(sys.argv)
    viewer=MainUI()
    viewer.show()
    
    sys.exit(app.exec_())