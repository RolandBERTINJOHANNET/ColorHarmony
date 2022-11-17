import scipy.io as io
import numpy as np

filename = "mturkData.mat"

mat = io.loadmat(filename)
palettes = np.array(mat["data"])
scores = np.array(mat["targets"])

print("shape of scores : ", scores.shape, "shape of palettes : ", palettes.shape)