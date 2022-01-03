import numpy as np
import sys
from sys import exit


x = np.fromfile("../data/bin/X.bin", dtype=float, count=-1)
print(x.shape[0], x)

y = np.fromfile("../data/bin/Y.bin", dtype=float, count=-1)
print(y.shape)
y = np.reshape(y, (5, 3))
print(y)

W = np.fromfile("../data/bin/W.bin", dtype='float32', count=-1)
W = np.reshape(W, (5, 3))
print(W)


z = np.fromfile("../data/bin/Z.bin", dtype=np.uint32, count=-1)
z = np.reshape(z, (5, 3))
print (z)
