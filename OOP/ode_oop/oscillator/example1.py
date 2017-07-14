#!/usr/bin/python
'''
x'' + b * x' + C * sin(t)=0
'''
from scipy.integrate import odeint
import pylab as pl
import numpy as np

def pend(y, t, b, c=5.0):
    theta, omega = y
    dydt = [omega, -b*omega - c*np.sin(theta)]
    return dydt

b = 0.25
#c = 5.0
t = np.linspace(0, 40, 401)
y0 = [np.pi - 0.1, 0.0]
sol = odeint(pend, y0, t, args=(b,))
pl.plot(t, sol[:, 0], 'b', label='theta(t)')
pl.plot(t, sol[:, 1], 'g', label='omega(t)')
pl.xlabel('t')
pl.grid()

rk = np.loadtxt("runge_kutta4.txt")
pl.plot(rk[:,0],rk[:,1],label="rk x")
pl.plot(rk[:,0],rk[:,2],label="rk y")
pl.legend(frameon=False, loc='best')

# eu = np.loadtxt("euler.txt")
# pl.plot(eu[:,0],eu[:,1],label="euler x")
# pl.plot(eu[:,0],eu[:,2],label="euler y")
# pl.legend(frameon=False, loc='best')

pl.show()
