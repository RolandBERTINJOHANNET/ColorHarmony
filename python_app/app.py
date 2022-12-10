import sys
#from PyQt5 import QtCore, QtGui, QtWidgets
#from PyQt5.QtWidgets import QApplication, QWidget, QInputDialog, QLineEdit, QFileDialog
#from PyQt5.QtGui import QIcon
from ctypes import *
import ctypes 

ctypes.CDLL('./libsampler.so', mode=1)
lib = cdll.LoadLibrary('./libsampler.so')
lib.Sampler_new.restype  = ctypes.c_void_p
lib.Sampler_resize_GAN.argtypes = [ctypes.c_void_p]
lib.Sampler_loadHarmo.argtypes = [ctypes.c_void_p,ctypes.c_void_p]
lib.Sampler_buildImage.argtypes = [ctypes.c_void_p]



class Sampler(object):
    def __init__(self,filename):

        self.obj = lib.Sampler_new(filename)

    def downscale(self):
        lib.Sampler_resize_GAN(self.obj)
    
    def loadHarmonizedImage(self,imgname):
        lib.Sampler_loadHarmo(self.obj,imgname)
    
    def upscale(self):
        lib.Sampler_buildImage(self.obj)
    

"""
class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(800, 600)
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.photo = QtWidgets.QLabel(self.centralwidget)
        self.photo.setGeometry(QtCore.QRect(0, 0, 841, 511))
        self.photo.setText("")
        self.photo.setPixmap(QtGui.QPixmap("cat.jpg"))
        self.photo.setScaledContents(True)
        self.photo.setObjectName("photo")
        self.cat = QtWidgets.QPushButton(self.centralwidget)
        self.cat.setGeometry(QtCore.QRect(0, 510, 411, 41))
        self.cat.setObjectName("cat")
        self.dog = QtWidgets.QPushButton(self.centralwidget)
        self.dog.setGeometry(QtCore.QRect(410, 510, 391, 41))
        self.dog.setObjectName("dog")
        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QtWidgets.QMenuBar(MainWindow)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 800, 21))
        self.menubar.setObjectName("menubar")
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QtWidgets.QStatusBar(MainWindow)
        self.statusbar.setObjectName("statusbar")
        MainWindow.setStatusBar(self.statusbar)

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

        self.dog.clicked.connect(self.show_dog)
        self.cat.clicked.connect(self.show_cat)
        self.dialog(MainWindow)

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "MainWindow"))
        self.cat.setText(_translate("MainWindow", "CAT"))
        self.dog.setText(_translate("MainWindow", "DOG"))

    def show_dog(self):
        self.photo.setPixmap(QtGui.QPixmap("imgs/dog.jpg"))

    def show_cat(self):
        self.photo.setPixmap(QtGui.QPixmap("imgs/cat.jpg"))
        
    def dialog(self,MainWindow):
        file , check = QFileDialog.getOpenFileName(MainWindow, "QFileDialog.getOpenFileName()",
                                               "", "All Files (*);;Png Files (*.png);;Jpg Files (*.jpg)")
        if check:
            return file
"""
if __name__ == "__main__":
    #app = QtWidgets.QApplication(sys.argv)
    #MainWindow = QtWidgets.QMainWindow()
    #ui = Ui_MainWindow()
    #ui.setupUi(MainWindow)
    #MainWindow.show()
    #sys.exit(app.exec_())
    sampler = Sampler(b"hd.jpg")
    print('here')
    sampler.downscale()
    print('here')
    sampler.loadHarmonizedImage(b'hued.png')
    print('here')
    sampler.upscale()
