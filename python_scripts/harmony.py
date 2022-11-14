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
import pandas as pd



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

def vote(vote1,vote2,vote3):#using votes 2 and 3 to check "impurities" in vote1
	if vote1==vote2:
		return vote1
	elif vote1==vote3:
		return vote1
	else:
		return "unknown"


data_dir_small = os.path.join(os.path.dirname(os.path.abspath(__file__)),"..\\script_database\\databaseOut3")
data_dir_big = os.path.join(os.path.dirname(os.path.abspath(__file__)),"..\\script_database\\databaseOut2")
out_dir =  os.path.join(os.path.dirname(os.path.abspath(__file__)),"..\\harmonies_database\\")

#big images must be 256 by 256 and small images must be 50 by 50 for this to work well
if __name__ == "__main__":
	i=0
	nb_conflicts = 0
	votes3 = pd.read_csv("../split_merge/harmony_types.csv")
	votes3.columns = ["name","type"]

	for file in os.listdir(data_dir_small):
		if file in votes3["name"].values:
			img = Image.open(data_dir_small+"/"+file).convert("RGB")
			bins_freq = getMainColors_frequencies(img)
			img = Image.open(data_dir_small+"/"+file).convert("RGB")
			bins_kmeans = getMainColors_kmeans(img)

			if bins_freq is not None and bins_kmeans is not None:
				vote1 = getHarmonyType(bins_freq)
				vote2 = getHarmonyType(bins_kmeans)
				vote3 = votes3.loc[votes3["name"]=="00000000_(2).jpg"]["type"].values[0]

				print("votes : ",vote1,vote2,vote3)

				result = vote(vote1,vote2,vote3)
				print("result : ",result)
				if result=="unknown":
					nb_conflicts+=1
				else:
					img = Image.open(data_dir_big+"/"+file).convert("RGB")
					print("\n",i,"images done : ",out_dir+result+"/"+file)
					i+=1
					img.save(out_dir+result+"/"+file)
	print("percentage of images where methods disagreed : ",100.*(nb_conflicts/float(len(os.listdir(data_dir_small)))))