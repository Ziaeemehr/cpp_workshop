import numpy as np
import sys
from sys import exit


x = np.fromfile("../data/bin/X.bin", dtype=float, count=-1)
print x

y = np.fromfile("../data/bin/Y.bin", dtype=float, count=-1)
y = np.reshape(y, (10,3))
print y

z = np.fromfile("../data/bin/Z.bin", dtype=np.uint32, count=-1)
z = np.reshape(z, (10, 3))
print z

