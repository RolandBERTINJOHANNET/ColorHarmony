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
from colorExtraction import *




def getHarmonyType(bins):
		#pick a harmony type
		if len(bins)==1:
			return "mono"
		elif len(bins)==2:
			return "comp" if abs(bins[0]-bins[1])>110 else "analog"
		elif len(bins)==3:
			return "triad"
		else:
			return "rectangle"

def draw_maincolors(img,bins):
	last_line = np.zeros((np.array(img).shape[0],100,3))
	for i in range(len(bins)):
		lch = LCHabColor(60.,80.,bins[i])
		RGB_color = convert_color(lch,sRGBColor)
		rgb = [RGB_color.rgb_r,RGB_color.rgb_g,RGB_color.rgb_b]
		for j in range(i*50,(i+1)*50):
			for k in range(100):
				last_line[j,k]=np.array([rgb[0]*255,rgb[1]*255,rgb[2]*360])
	final = np.concatenate([np.array(img),last_line],axis=1)
	return Image.fromarray(final.astype(np.uint8))




data_dir_small = os.path.join(os.path.dirname(os.path.abspath(__file__)),"..\\script_database\\databaseOut3")
data_dir_big = os.path.join(os.path.dirname(os.path.abspath(__file__)),"..\\script_database\\databaseOut2")
out_dir =  os.path.join(os.path.dirname(os.path.abspath(__file__)),"..\\harmonies_database\\")

#big images must be 256 by 256 and small images must be 50 by 50 for this to work well
if __name__ == "__main__":
	i=0
	for file in os.listdir(data_dir_small):
		img = Image.open(data_dir_small+"/"+file).convert("RGB")
		bins = getMainColors_frequencies(img)
		if bins is not None:
			harmonyType = getHarmonyType(bins)
			#print("final choice was ",harmonyType)
			img = Image.open(data_dir_big+"/"+file).convert("RGB")
			img = draw_maincolors(img,bins)
			print("\n",i,"images done : ",out_dir+harmonyType+"/"+file)
			i+=1
			img.save(out_dir+harmonyType+"/"+file)