import functions as fn
import numpy as np 
import pylab as pl 

sol = fn.integrate([3.1315926535897933,0.0], 4000 , 0.01)


sol = np.array(sol)
print type(sol)
print sol.shape
pl.plot(sol[:,0],label='x')
pl.plot(sol[:,1],label='y')
pl.show()



