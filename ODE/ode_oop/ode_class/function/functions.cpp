#include "functions.h"

/*-------------------------------------------------------*/
StateVec sysRHS (StateVec &y) 
{ 
  StateVec K(2);
  K[0] = y[1];
  K[1] = -0.25 * y[1] - 5.0*sin(y[0]);
  return K;
}

/*-------------------------------------------------------*/
void euler_integrator (StateVec &y, double dt) {
  
  StateVec f(2);
  f = sysRHS(y);

  for (int i=0; i<y.size(); i++)
    y[i] += f[i] * dt;
}
/*-------------------------------------------------------*/
StateVec2 integrate (  StateVec y, int num_steps, double dt) //std::ofstream &output,
{  

  StateVec2 coordinates(num_steps,StateVec(3));

  for (int step = 0; step < num_steps; ++step) {
    coordinates[step][0] = step*dt;
    for (int j=0; j<2; j++)
       coordinates[step][j+1] = y[j];

    euler_integrator (y, dt);
  }
  return coordinates;
}
/*-------------------------------------------------------*/