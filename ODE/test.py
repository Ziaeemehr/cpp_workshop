import pylab as pl 
import numpy as np 

eul = np.loadtxt("euler.txt")
rk = np.loadtxt('rk4.txt')
print eul.shape
print rk.shape

pl.plot(eul[:,0], np.sin(eul[:,1]))
pl.plot(rk[:,0], np.sin(rk[:,1]))

pl.show()
