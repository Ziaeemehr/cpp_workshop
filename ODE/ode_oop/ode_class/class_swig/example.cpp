/* File : example.cxx */

#include "example.h"
#define M_PI 3.14159265358979323846

/*------------------------------------------------------------*/
std::vector<StateVec > ODE::integrate () 
{   
    std::vector<StateVec > Coordinates(num_steps,StateVec(N+1));
    StateVec y = IC;

    for (int step = 0; step < num_steps; ++step) 
    {
        Coordinates[step][0] = step*dt;
        for (int j=1; j<N+1; j++)
            Coordinates[step][j] = y[j-1];

        // euler_integrator(y);
        runge_kutta4_integrator(y);
    }
    return Coordinates;
}
/*------------------------------------------------------------*/
void ODE::euler_integrator (StateVec &y )
{
    StateVec f(N);
    f = dydt(y);
    for (int i=0; i<y.size(); i++)
        y[i] += f[i] * dt;
}
/*------------------------------------------------------------*/
void ODE::runge_kutta4_integrator (StateVec &y) 
{
    int n = y.size();
    StateVec k1(n), k2(n), k3(n), k4(n);
    StateVec f(n);

    k1 = dydt(y);

    for (int i=0; i<n; i++)
        f[i]= y[i]+ 0.5 * dt * k1[i];
    k2 = dydt (f);

    for (int i=0; i<n; i++)
        f[i]= y[i]+ 0.5 * dt * k2[i];
    k3 = dydt(f);

    for (int i=0; i<n; i++)
        f[i]= y[i]+ dt * k3[i];
    k4 = dydt(f);

    for (int i=0; i<n; i++)
        y[i] += (k1[i] + 2.0*(k2[i] + k3[i]) + k4[i]) * dt/6.0;
}
/*------------------------------------------------------------*/
StateVec ODE::dydt(const StateVec &x)
{
    double sumx = 0.0;
    int n = N;
    StateVec f(n);
    for (int i=0; i<n; i++)
    {
        sumx = 0;
        for(int j=0; j<n; j++)
            sumx += sin(x[j]-x[i]);

        f[i] = M_PI/2.0 + 1./n * sumx;
    }

    return f;
}