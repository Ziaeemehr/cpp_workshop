#include "functions.h"

using namespace std;

constexpr double    dt = 1.e-2;
constexpr int     PARN = 50;
constexpr double PARka = 2.0;
constexpr double PARka_over_N = PARka/(double)PARN;
constexpr double PARw = M_PI;
constexpr auto num_steps = 4000;


StateVec Kuramoto (const StateVec &x) 
{
  double sumx=0;
  int N = PARN;
  StateVec f;
  f = StateVec(N);
  #pragma omp parallel for reduction(+:sumx)
  for (int i=0; i<N; i++){
      sumx = 0;
      for(int j=0; j<N; j++)
          sumx += sin(x[j]-x[i]);
      f[i] = PARw + PARka_over_N * sumx;
  }
  return f;
}

void print_coordinates (double time, const StateVec &y, std::ofstream &output) {
    output <<std::setprecision(10);
    int N = PARN;
    for (int i=0; i<N; i++)
    {
      output <<std::setw(20) << sin(y[i]);
    }
    output<< std::endl;  
}


int main (int argc, char* argv[]) {
  ofstream euler_output {"euler.txt"};
  ofstream runge_kutta4_output {"runge_kutta4.txt"};

  StateVec initial_state,PARw;
  initial_state = StateVec(PARN);
    
  for(int i=0; i<PARN; i++) {
    initial_state[i] =  (double) rand()/(RAND_MAX) * 2*M_PI;
  }
  integrate(euler_integrator, Kuramoto, print_coordinates, euler_output, initial_state, num_steps, dt);
  integrate(runge_kutta4_integrator, Kuramoto, print_coordinates, runge_kutta4_output, initial_state, num_steps, dt);

  return 0;
}
