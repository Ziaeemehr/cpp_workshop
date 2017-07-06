import pylab as pl 
import numpy as np 

# signal = np.loadtxt("signal.txt")
# pl.plot(signal[:,0],signal[:,1])

pl.figure()
result = np.loadtxt("result.txt")
pl.plot(result[:,0],result[:,1])
# pl.plot(result[:,1])


# pl.figure()
# mag = np.loadtxt("magnitude.txt")
# pl.plot(mag)
pl.savefig("f.png")
pl.show()
