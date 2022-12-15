import sys
from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtWidgets import QApplication, QWidget, QInputDialog, QLineEdit, QFileDialog
from PyQt5.QtGui import QIcon
from ctypes import *
from PIL import Image, ImageQt
import numpy as np
from runModel import *
import ctypes 

ctypes.CDLL('./SamplerDLL.dll', mode=1)
lib = cdll.LoadLibrary('././SamplerDLL.dll')
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
    
class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        self.mw = MainWindow
        MainWindow.setObjectName("ColorHarmony")
        MainWindow.resize(1200, 600)
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.photo = QtWidgets.QLabel(self.centralwidget)
        self.photo.setGeometry(QtCore.QRect(0, 80, 600, 400))
        self.photo.setText("")
        self.photo.setScaledContents(True)
        self.photo.setObjectName("photo")
        
        self.result = QtWidgets.QLabel(self.centralwidget)
        self.result.setGeometry(QtCore.QRect(600, 80, 600, 400))
        self.result.setText("")
        self.result.setScaledContents(True)
        self.result.setObjectName("result")
        
        self.open = QtWidgets.QPushButton(self.centralwidget)
        self.open.setGeometry(QtCore.QRect(500, 15, 200, 50))
        self.open.setObjectName("Ouvrir image")
        
        self.mono = QtWidgets.QPushButton(self.centralwidget)
        self.mono.setGeometry(QtCore.QRect(0, 500, 240, 80))
        self.mono.setObjectName("monochromatique")
        self.anal = QtWidgets.QPushButton(self.centralwidget)
        self.anal.setGeometry(QtCore.QRect(240, 500, 240, 80))
        self.anal.setObjectName("analogue")
        self.comp = QtWidgets.QPushButton(self.centralwidget)
        self.comp.setGeometry(QtCore.QRect(480, 500, 240, 80))
        self.comp.setObjectName("complémentaire")
        self.tri = QtWidgets.QPushButton(self.centralwidget)
        self.tri.setGeometry(QtCore.QRect(720, 500, 240, 80))
        self.tri.setObjectName("triangulaire")
        self.rect = QtWidgets.QPushButton(self.centralwidget)
        self.rect.setGeometry(QtCore.QRect(960, 500, 240, 80))
        self.rect.setObjectName("rectangle")
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

        self.mono.clicked.connect(self.apply_mono)
        self.anal.clicked.connect(self.apply_anal)
        self.comp.clicked.connect(self.apply_comp)
        self.tri.clicked.connect(self.apply_tri)
        self.rect.clicked.connect(self.apply_rect)
        self.open.clicked.connect(self.open_image)
        
    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "ColorHarmony"))
        self.open.setText(_translate("ColorHarmony", "Ouvrir image"))
        self.mono.setText(_translate("ColorHarmony", "monochromatique"))
        self.anal.setText(_translate("ColorHarmony", "analogue"))
        self.comp.setText(_translate("ColorHarmony", "complémentaire"))
        self.tri.setText(_translate("ColorHarmony", "triadique"))
        self.rect.setText(_translate("ColorHarmony", "rectangle"))
    
    def apply_mono(self):
        self.image = np.array(Image.open("downscaled.png"))
        table = run_model("mono",self.image)
        print(np.min(table),np.max(table))
        np.shape(table)
        img = Image.fromarray((table*255.).astype(np.uint8))
        img.save("temp.png")
        self.sampler.loadHarmonizedImage(b'temp.png')
        self.sampler.upscale()
        self.qt_res = ImageQt.ImageQt(Image.open("result.png"))
        self.result.setPixmap(QtGui.QPixmap.fromImage(self.qt_res))
        
    def apply_anal(self):
        self.image = np.array(Image.open("downscaled.png"))
        table = run_model("analog",self.image)
        print(np.min(table),np.max(table))
        np.shape(table)
        img = Image.fromarray((table*255.).astype(np.uint8))
        img.save("temp.png")
        self.sampler.loadHarmonizedImage(b'temp.png')
        self.sampler.upscale()
        self.qt_res = ImageQt.ImageQt(Image.open("result.png"))
        self.result.setPixmap(QtGui.QPixmap.fromImage(self.qt_res))
        
    def apply_comp(self):
        self.image = np.array(Image.open("downscaled.png"))
        table = run_model("comp_wasserstein",self.image)
        print(np.min(table),np.max(table))
        np.shape(table)
        img = Image.fromarray((table*255.).astype(np.uint8))
        img.save("temp.png")
        self.sampler.loadHarmonizedImage(b'temp.png')
        self.sampler.upscale()
        self.qt_res = ImageQt.ImageQt(Image.open("result.png"))
        self.result.setPixmap(QtGui.QPixmap.fromImage(self.qt_res))
    def apply_tri(self):
        self.image = np.array(Image.open("downscaled.png"))
        table = run_model("triad",self.image)
        print(np.min(table),np.max(table))
        np.shape(table)
        img = Image.fromarray((table*255.).astype(np.uint8))
        img.save("temp.png")
        self.sampler.loadHarmonizedImage(b'temp.png')
        self.sampler.upscale()
        self.qt_res = ImageQt.ImageQt(Image.open("result.png"))
        self.result.setPixmap(QtGui.QPixmap.fromImage(self.qt_res))
    def apply_rect(self):
        self.image = np.array(Image.open("downscaled.png"))
        table = run_model("rect_wasserstein",self.image)
        print(np.min(table),np.max(table))
        np.shape(table)
        img = Image.fromarray((table*255.).astype(np.uint8))
        img.save("temp.png")
        self.sampler.loadHarmonizedImage(b'temp.png')
        self.sampler.upscale()
        self.qt_res = ImageQt.ImageQt(Image.open("result.png"))
        self.result.setPixmap(QtGui.QPixmap.fromImage(self.qt_res))
    def open_image(self):
        print("dialog ouvert")
        file , check = QFileDialog.getOpenFileName(self.mw, "QFileDialog.getOpenFileName()",
                                               "", "Png Files (*.png);;Jpg Files (*.jpg)")
        if check:
            self.filename = file
            print("image trouvee")
            img = Image.open(file)
            print("image ouverte"+str(img.size[0])+" "+str(img.size[1]))
            self.qt_img = ImageQt.ImageQt(img)
            print("image convertie")
            self.photo.setPixmap(QtGui.QPixmap.fromImage(self.qt_img))
            print("image set")

            self.sampler = Sampler(bytes(file,'UTF-8'))
            self.sampler.downscale()

        

        
    def dialog(self,MainWindow):
        file , check = QFileDialog.getOpenFileName(MainWindow, "QFileDialog.getOpenFileName()",
                                               "", "All Files (*);;Png Files (*.png);;Jpg Files (*.jpg)")
        if check:
            self.filename = file


if __name__ == "__main__":
    app = QtWidgets.QApplication(sys.argv)
    MainWindow = QtWidgets.QMainWindow()
    ui = Ui_MainWindow()
    ui.setupUi(MainWindow)
    MainWindow.show()
    sys.exit(app.exec_())