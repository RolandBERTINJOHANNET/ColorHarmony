import scipy.io as io
import numpy as np

filename = "mturkData.mat"

mat = io.loadmat(filename)
palettes = np.array(mat["data"])
scores = np.array(mat["targets"])

print("shape of scores : ", scores.shape, "shape of palettes : ", palettes.shape)
print("first line of scores : ",scores[0:5])
print("first line of palettes : ",palettes[0,0,:])

#reshape to fit into a csv file
palettes = palettes.reshape((palettes.shape[0],-1))
scores = scores.reshape((scores.shape[0],-1))

#save to file
np.savetxt("palettes.csv", palettes, delimiter=",")
np.savetxt("scores.csv", scores, delimiter=",")

#verify they can be loaded back
palettes_shape = np.array(mat["data"]).shape
scores_shape = np.array(mat["targets"]).shape

palettes = np.loadtxt("palettes.csv", delimiter=",").reshape(palettes_shape)
scores = np.loadtxt("scores.csv", delimiter=",").reshape(scores_shape)

print("shape of scores from files: ", scores.shape, "shape of palettes from files: ", palettes.shape)
print("first line of scores : ",scores[0:5])
print("first line of palettes : ",palettes[0,0,:])