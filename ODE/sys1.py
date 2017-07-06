import numpy as np
import matplotlib.pyplot as plt
from scipy.integrate import odeint

def xprsys(ic,t):
    dx = np.zeros(N)
    x = ic
    for i in range(N):
        dx[i] = w[i]+ka*np.sum(np.sin(x-x[i]))
    return dx


N  = 30
ka = 2.0
tfinal = 20.0
dt = 0.01 
ic = np.array([i*0.1 for i in range(N)])
t = np.arange(0, tfinal+dt,dt)
nstep = len(t)
w = np.ones(N) * np.pi
sol = odeint(xprsys,ic,t)

ofile = open("odeint.txt", "w")
for i in range(nstep):
    ofile.write("%22.14f"%t[i])
    for j in range(N):
        ofile.write("%22.14f"%sol[i,j])
    ofile.write("\n")

comp = np.loadtxt("rk4.txt")
plt.plot(t, np.sin(sol[:, 0]), label='odeint')
plt.plot(comp[:,0], np.sin(comp[:, 1]), label='comp')
plt.legend(loc='best')
plt.xlabel('t')
plt.grid()
plt.show()
