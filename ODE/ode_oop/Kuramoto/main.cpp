#include "functions.h"

using namespace std;


int main (int argc, char* argv[]) {
  ofstream euler_output {"euler.txt"};
  ofstream runge_kutta4_output {"runge_kutta4.txt"};

  StateVec initial_state(PARN);
  StateVec PARw;
    
  for(int i=0; i<PARN; i++) {
    initial_state[i] =  (double) rand()/(RAND_MAX) * 2*M_PI;
  }
  integrate(euler_integrator, Kuramoto, print_coordinates, 
            euler_output, initial_state, num_steps, dt);
            
  integrate(runge_kutta4_integrator, Kuramoto, print_coordinates,
            runge_kutta4_output, initial_state, num_steps, dt);

  return 0;
}
