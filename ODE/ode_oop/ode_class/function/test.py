import functions as fn
import numpy as np 
import pylab as pl 

dt = 0.01
initial_conditions = [np.pi-0.1,0.0]
sol = fn.integrate(initial_conditions, int(40/dt) , dt)

sol = np.array(sol)
print type(sol)
print sol.shape
pl.plot(sol[:,0], sol[:,1],label='x')
pl.plot(sol[:,0], sol[:,2],label='y')
pl.show()



