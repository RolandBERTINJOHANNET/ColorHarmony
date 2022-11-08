from PIL import Image, ImageOps
import os

dir = "C:/Users/Orlando/Desktop/script_database/databaseOut"
dirOut = "C:/Users/Orlando/Desktop/script_database/databaseOut2"
size = 50
print("running")

for file in os.listdir(dir):
    if file.endswith(".jpg"):
        img = Image.open(os.path.join(dir, file))
        img = img.resize((size,size))
        print("doing ",os.path.join(dirOut, file)[:-4]+".jpg")
        img.save(os.path.join(dirOut, file)[:-4]+".jpg")