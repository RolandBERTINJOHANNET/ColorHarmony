# -*- coding: utf-8 -*-
"""
Created on Mon Oct 31 14:45:42 2022

@author: Orlando
"""
import os
import PIL.Image as Image
from scipy.cluster.vq import kmeans
import numpy as np
import colorconversion

def getHarmonyType(img):
	#extract hues and select those that matter
	hues = []
	img = np.array(img)
	img = np.reshape(img,(img.shape[0]*img.shape[1],3))
	for pixel in np.array(img):
		LAB_color = colorconversion.RGB_to_LAB(pixel[0],pixel[1],pixel[2])
		LCH_color = colorconversion.LAB_to_LCH(LAB_color[0],LAB_color[1],LAB_color[2])
		if LCH_color[0]>0.1 and LCH_color[1]>0.1:
			hues.append(LCH_color[2])
		
	#kmeans
	if(len(hues)>1):
		colors,_ = kmeans(hues,4)
		colors = list(colors)
		
		
		#grouping of colors
		bins=[]
		i=0
		while i<len(colors):
			hue = colors[i]
			similar_colors = [colors[i]]
			colors.remove(hue)
			j=0
			while j<len(colors):
				if abs(hue - colors[j])<55:
					similar_colors.append(colors[j])
					colors.remove(colors[j])
				else:
					j+=1
			bins.append(np.mean(similar_colors))
		
		
	
		#pick a harmony type
		if len(bins)==1:
			return "mono"
		elif len(bins)==2:
			return "comp" if abs(bins[0]-bins[1])>90 else "analog"
		elif len(bins)==3:
			return "triad"
		else:
			return "rectangle"



data_dir_small = "C:/Users/Orlando/Desktop/temp_harmonisation/script_database/databaseOut2"
data_dir_big = "C:/Users/Orlando/Desktop/temp_harmonisation/script_database/databaseOut"
out_dir =  "C:/Users/Orlando/Desktop/temp_harmonisation/script_database/harmonies_database/"

if __name__ == "__main__":
	i=0
	for file in os.listdir("databaseOut2"):
		img = Image.open(data_dir_small+"/"+file).convert("RGB")
		harmonyType = getHarmonyType(img)
		if harmonyType is not None:
			img = Image.open(data_dir_big+"/"+file).convert("RGB")
			print(i,"images done : ",out_dir+harmonyType+"/"+file)
			i+=1
			img.save(out_dir+harmonyType+"/"+file)
	