import numpy as np 
from sys import exit
import sys


a = np.fromfile("numbers.bin", dtype=np.uint32, count=-1)
print a.shape
a = np.reshape(a, (20,15))
for i in range(20):
	for j in range(15):
		print a[i,j],
	print " "


# a = np.fromfile("numbers.bin", dtype=float, count=-1)
# print a.shape
# a = np.reshape(a, (a.shape[0]/3,3))
# print a.shape
# print a