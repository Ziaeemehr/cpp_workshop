import numpy as np

def pend(y, t, gam):
    theta, omega = y
    dydt = [omega, -theta -gam * omega]
    return dydt

gam = 0.15
y0 = [1.0, 0.0]

t = np.linspace(0, 10, 10001)

from scipy.integrate import odeint
sol = odeint(pend, y0, t, args=(gam,))

import matplotlib.pyplot as plt
plt.plot(t, sol[:, 0], label='$\theta$, Py')
#plt.plot(t, sol[:, 1], 'g', label='omega(t)')
ifile = open('result.txt','r')
data = np.genfromtxt(ifile,dtype='float')
plt.plot(data[:,0],data[:,1],label='$\theta$,C++')

plt.legend(loc='best')
plt.xlabel('t')
plt.grid()
plt.show()