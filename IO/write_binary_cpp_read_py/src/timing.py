import numpy as np
from sys import exit
from time import time


N = 2000

start1 = time()
y = np.fromfile("../data/bin/X.bin", dtype=float, count=-1)
y = np.reshape(y, (N,N))
end1 = time()-start1
print  "time %g  seconds " %  end1

    
start2 = time()
z = np.genfromtxt("../data/bin/Y.txt")
end2 = time()-start2
print "time %g  seconds " % end2

print "speedup " , end2/end1 

print "%15s %15s" %("Y", "Z")
for i in range(10):
    print "%15.6f %15.6f " % (y[i,0], z[i,0])

