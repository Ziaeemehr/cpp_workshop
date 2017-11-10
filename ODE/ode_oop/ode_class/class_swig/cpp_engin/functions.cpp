/* File : functions.cpp */

#include "functions.h"
#define M_PI 3.14159265358979323846

/*------------------------------------------------------------*/
std::vector<StateVec > ODE::integrate (int num_steps) 
{   
    std::vector<StateVec > Coordinates(num_steps,StateVec(N+1));
    StateVec y = IC;

    for (int step = 0; step < num_steps; ++step) 
    {
        Coordinates[step][0] = step*dt;
        for (int j=1; j<N+1; j++)
            Coordinates[step][j] = y[j-1];

        euler_integrator(y);
    }
    return Coordinates;
}
/*------------------------------------------------------------*/
void ODE::euler_integrator (StateVec &y )
{
    StateVec f(N);
    f = oscillator(y);
    for (int i=0; i<y.size(); i++)
        y[i] += f[i] * dt;
}
/*------------------------------------------------------------*/
StateVec ODE::oscillator(const StateVec &x)
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