from PIL import Image, ImageOps
import os

dir = os.path.join(os.path.dirname(os.path.abspath(__file__)),"..\\harmonies_database\\comp")
dirOut = os.path.join(os.path.dirname(os.path.abspath(__file__)),"..\\harmonies_database\\temp\\comp_small")
size = 50
print("running")

for file in os.listdir(dir):
    if file.endswith(".jpg") or file.endswith(".png"):
        img = Image.open(os.path.join(dir, file))
        img = img.resize((size,size))
        print("doing ",os.path.join(dirOut, file)[:-4]+".jpg")
        img.save(os.path.join(dirOut, file)[:-4]+".jpg")