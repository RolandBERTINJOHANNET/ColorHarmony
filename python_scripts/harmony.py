# -*- coding: utf-8 -*-
"""
Created on Mon Oct 31 14:45:42 2022

@author: Orlando
"""
import os
import PIL.Image as Image
from sklearn.cluster import KMeans
import numpy as np

from colormath.color_objects import LCHabColor, sRGBColor
from colormath.color_conversions import convert_color



def getHarmonyType(img):
	#extract hues and select those that matter
	hues = []
	img = np.array(img)
	img = np.reshape(img,(img.shape[0]*img.shape[1],3))
	for pixel in np.array(img):
		rgb = sRGBColor(pixel[0],pixel[1],pixel[2])
		LCH_color = convert_color(rgb, LCHabColor)
		lch = (LCH_color.lch_l,LCH_color.lch_c,LCH_color.lch_h)
		if lch[0]>20 and lch[1]>20:
			hues.append(lch[2])
		
	#kmeans
	if(len(hues)>1):
		hues = np.reshape(np.array(hues),(len(hues),1))
		km = KMeans(n_clusters=4).fit(hues)
		pred = km.predict(hues)
		print("prediction : ",pred)
		for k in range(4):
			print("cluster",k,"has",len(np.where(pred==k)[0]),"and center",km.cluster_centers_[k])
		return
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


data_dir_small = os.path.join(os.path.dirname(os.path.abspath(__file__)),"..\\script_database\\databaseOut3")
data_dir_big = os.path.join(os.path.dirname(os.path.abspath(__file__)),"..\\script_database\\databaseOut2")
out_dir =  os.path.join(os.path.dirname(os.path.abspath(__file__)),"..\\harmonies_database\\")

if __name__ == "__main__":
	i=0
	for file in os.listdir(data_dir_small):
		img = Image.open(data_dir_small+"/"+file).convert("RGB")
		harmonyType = getHarmonyType(img)
		if harmonyType is not None:
			img = Image.open(data_dir_big+"/"+file).convert("RGB")
			print(i,"images done : ",out_dir+harmonyType+"/"+file)
			i+=1
			img.save(out_dir+harmonyType+"/"+file)