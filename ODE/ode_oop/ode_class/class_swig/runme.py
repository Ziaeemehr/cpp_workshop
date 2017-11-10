# file: runme.py

# This file illustrates the proxy class C++ interface generated
# by SWIG.

import example
import pylab as pl 
import numpy as np 

# ----- Object creation -----

c = example.ODE(2, 40, 0.01, [np.pi-.01,0])

sol = c.integrate()

sol = np.array(sol)
pl.plot(sol[:,0], np.sin(sol[:,1]))
pl.plot(sol[:,0], np.sin(sol[:,2]))
pl.show()

del c

