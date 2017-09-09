#include "dde.h"
/*------------------------------------------------------------*/
void dde::euler_integrator (StateVec &y, DerivFunc dydt )
{
  y += (this->*dydt) (y) * dt;
}
/*------------------------------------------------------------*/
void dde::integrate (Integrator integrator, DerivFunc dydt) 
{   
    StateVec y = IC;

    for (auto step = 0; step < 1000; ++step) 
    {
        std::cout << step*dt << "\t"<< y[0]<< " "<<y[1] <<"\n";
        (this->*integrator)(y, dydt);
    }
}
/*------------------------------------------------------------*/
StateVec dde::oscillator(const StateVec &x)
{
    double sumx = 0.0;
    int n = N;
    StateVec f(n);
    for (int i=0; i<n; i++){
        sumx = 0;
        for(int j=0; j<n; j++)
            sumx += sin(x[j]-x[i]);
        f[i] = M_PI/2.0 + 1./n * sumx;
    }

  return f;
}
/*------------------------------------------------------------*/