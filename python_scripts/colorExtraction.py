import os
import PIL.Image as Image
from sklearn.cluster import KMeans
import numpy as np

from colormath.color_objects import LCHabColor, sRGBColor
from colormath.color_conversions import convert_color
from colorExtraction import *

COLOR_REJECTION_THRESHOLD = 1./10.
GROUPING_ANGLE = 20

def getMainColors_kmeans(img):
	#extract hues and select those that matter
	hues = []
	if type(img)!=type(np.zeros(0)):
		img = np.array(img)
		img = np.reshape(img,(img.shape[0]*img.shape[1],3))/255.
	for pixel in np.array(img):
		rgb = sRGBColor(pixel[0],pixel[1],pixel[2])
		LCH_color = convert_color(rgb, LCHabColor)
		lch = (LCH_color.lch_l,LCH_color.lch_c,LCH_color.lch_h)
		if lch[0]>10 and lch[1]>10:
			hues.append(lch[2])
		
	#kmeans
	if(len(hues)>200):#if enough hues are associated with enough saturation and luminance to give good estimate of color harmony
		hues = np.reshape(np.array(hues),(len(hues),1))
		km = KMeans(n_clusters=4).fit(hues)
		pred = km.predict(hues)
		cluster_sizes=[]
		colors=[]
		for k in range(4):
			cluster_sizes.append(len(np.where(pred==k)[0]))
			colors.append(km.cluster_centers_[k])
			#print("cluster",k,"has",len(np.where(pred==k)[0]),"and center",km.cluster_centers_[k])
		
		
		#grouping of colors
		bins=[]
		bins_sizes=[]
		i=0
		while i<len(colors):
			hue = colors[i]
			similar_colors = [colors[i]]
			sim_cols_clsiz = [cluster_sizes[i]]
			colors.remove(hue)
			cluster_sizes.remove(cluster_sizes[i])
			j=0
			while j<len(colors):
				if abs(hue - colors[j])<GROUPING_ANGLE:
					similar_colors.append(colors[j])
					sim_cols_clsiz.append(cluster_sizes[j])
					colors.remove(colors[j])
					cluster_sizes.remove(cluster_sizes[j])
				else:
					j+=1
			bins.append(np.mean(similar_colors))
			bins_sizes.append(sum(sim_cols_clsiz))
		#print("after similarity check : ",bins)
		#print("cluster sizes : ",bins_sizes)


		#take only the sufficiently big clusters.
		bins = [b for (b,c) in zip(bins,bins_sizes) if c>len(hues)*COLOR_REJECTION_THRESHOLD]
		#print("after elimination of small clusters : ",bins)
		return bins
	else:
		print("skipped image")
		return None


def getMainColors_frequencies(img):
	#extract hues and select those that matter
	hues = []
	if type(img)!=type(np.zeros(0)):
		img = np.array(img)
		img = np.reshape(img,(img.shape[0]*img.shape[1],3))/255.
	for pixel in np.array(img):
		rgb = sRGBColor(pixel[0],pixel[1],pixel[2])
		LCH_color = convert_color(rgb, LCHabColor)
		lch = (LCH_color.lch_l,LCH_color.lch_c,LCH_color.lch_h)
		if lch[0]>10 and lch[1]>10:
			hues.append(lch[2])


	if(len(hues)>200):#if enough hues are associated with enough saturation and luminance to give good estimate of harmony type
		#make 8-bin histogram
		nb_bins=16
		ratio = 360./float(nb_bins)
		hist = np.zeros(nb_bins)
		for hue in hues:
			hist[int(hue/ratio)]+=1

		#extract top 4
		idxsort = np.argsort(hist,axis=0)
		frequencies = list(hist[idxsort])[-4:]
		colors = list(idxsort[-4:] * ratio)
		#print("idxsort : ",idxsort)
		#print("colors",colors,"with frequencies",frequencies)

		
		
		


		#take only the sufficiently big clusters.
		bins = [b for (b,c) in zip(colors,frequencies) if c>len(hues)*COLOR_REJECTION_THRESHOLD]
		#print("after elimination of small bins : ",bins)
		return bins
	else:
		print("skipped image")
		return None