import numpy as np 
import pylab as pl 

fig, ax = pl.subplots(1, figsize=(8, 6))

r = np.loadtxt("r.txt")
n = len(r)

pl.hist(r, bins=int(np.sqrt(n)), color="k", density=True)
pl.savefig("f.png")
pl.show()