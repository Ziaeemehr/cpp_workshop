# file: runme.py

# This file illustrates the proxy class C++ interface generated
# by SWIG.

import example
import pylab as pl 
import numpy as np 
from time  import time

# ----- Object creation -----

start = time()

c = example.ODE(20, 400, 0.01, [np.pi-.01,0])

sol = c.integrate("euler")

print "Done in ", time()-start," seconds"

sol = np.array(sol)
pl.plot(sol[:,0], np.sin(sol[:,1]))
pl.plot(sol[:,0], np.sin(sol[:,2]))


pl.show()

del c

