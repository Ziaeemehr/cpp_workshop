import numpy as np 
from scipy.signal import hilbert


in_arr = np.asarray([1,2,3,4,5,6,7])
out_arr = hilbert(in_arr)

for i in out_arr:
    print i
