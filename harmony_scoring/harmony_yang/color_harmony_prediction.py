import scipy.io as io
import numpy as np
from colormath.color_objects import LCHabColor, sRGBColor
from colormath.color_conversions import convert_color

filename = "kulerData.mat"

#load data

mat = io.loadmat(filename)

palettes_shape = np.array(mat["data"]).shape
scores_shape = np.array(mat["targets"]).shape

palettes = np.loadtxt("palettesKuler.csv", delimiter=",").reshape(palettes_shape)
scores = np.loadtxt("scoresKuler.csv", delimiter=",").reshape(scores_shape)

#replace palette colors with LCH hues
huePalettes = []
for p in palettes:
	newColors = []
	for i in range(5):
		rgb = sRGBColor(p[i,0],p[i,1],p[i,2])
		lch = convert_color(rgb, LCHabColor)
		newColors.append(lch.lch_h)
	huePalettes.append(newColors)
huePalettes = np.array(huePalettes)

#get list of color pairs with their scores
pairScores = []
pairs = []
a=0
nb_repetitions = 0
for p,s in list(zip(huePalettes,scores)):
	a+=1
	print("doing ",a,"th pair")
	#get pairs
	thisPairs = []
	for i in range(4):
		thisPairs.append(np.array([p[i],p[i+1]]).astype(np.int16))
	#add to pairs while checking that there aren't any doubles
	for pair in thisPairs:
		#print("testing for pair : \n",pair)
		if len(pairs)==0 or not (pair==np.array(pairs)).all((1)).any():
			pairs.append(pair)
			pairScores.append([s])
		else:
			nb_repetitions+=1
			#find the index of the existing pair and add this score to it.
			find=0
			#print("pair exists................................................................. looking for pair\n",pair)
			while (pairs[find]!=pair).any():
				find+=1
			#print("found pair",pair,"at index",find)
			pairScores[find].append(s)

print("number of pairs : ",len(pairs))
print("number of repetitions : ",nb_repetitions)

#get the MLE harmony prediction for each pair
pairPlusPrediction = []
for p,sc in list(zip(pairs,pairScores)):
	pairPlusPrediction.append(np.array([p[0],p[1],np.mean((sc))]).astype(np.float16))
pairPlusPrediction = np.array(pairPlusPrediction)

#write it to a file
np.savetxt("pairPlusPredictionKuler.csv",pairPlusPrediction,delimiter=",")