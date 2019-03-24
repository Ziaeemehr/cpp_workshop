# file: runme.py

# This file illustrates the proxy class C++ interface generated
# by SWIG.

import odesolver
import pylab as pl 
import numpy as np 
from time  import time
from numpy import pi

# ----- Object creation -----

start = time()
N = 20
IC = np.random.uniform(-pi, pi, N).tolist()
c = odesolver.ODE(N, 400.0, 0.01, IC)

sol = c.integrate("euler")

print "Done in ", time()-start," seconds"

sol = np.array(sol)
pl.plot(sol[:,0], np.sin(sol[:,1]))
pl.plot(sol[:,0], np.sin(sol[:,2]))


pl.show()

del c

