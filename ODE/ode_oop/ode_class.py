#!/usr/bin/env python

import pylab as pl
import numpy as np
from math import sin

class integrate: 
    def __init__(self,IC,t):
        self.IC = IC 
        self.t = t 
        self.x = [0]*(len(self.t))
    def euler(self,f):
        nstep = len(self.t)
        self.x[0] = self.IC
        dt = self.t[1]-self.t[0]
        for i in range(nstep-1):
            self.x[i+1] = self.x[i] + f(self.t[i])*dt
        return self.x
    def display(self):
        pl.plot(self.t, self.x, 'b', label='theta(t)')
        pl.legend(frameon=False, loc='best')
        pl.xlabel('t')
        pl.grid()
        
def f(t):
    return sin(t)

t = [i*0.001 for i in range(10000)]
ode = integrate(-1,t)
sol = ode.euler(f)
ode.display()
pl.plot(t[0:-1:100],-1*np.cos(t[0:-1:100]),"ro",label="exact")
pl.legend()
pl.show()
