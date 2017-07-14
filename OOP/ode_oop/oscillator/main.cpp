#include "functions.h"

using namespace std;

constexpr auto dt = 1.e-2;
constexpr auto PARb = 0.25;
constexpr auto PARc = 5.0;
StateVec initial_conditions = {M_PI-0.1,0.0};
constexpr auto num_steps = 4000;


StateVec sysRHS (const StateVec &y) {
  return valarray<double> {y[1], -PARb*y[1]-PARc*sin(y[0])};
}

void print_coordinates (double time, const StateVec &y, std::ofstream &output) {
    output <<std::setprecision(10);
    output <<std::setw(20)<< time << std::setw(20) << y[0] << std::setw(20) << y[1] << '\t' << std::endl;
}



int main (int argc, char* argv[]) {
  ofstream euler_output {"euler.txt"};
  ofstream runge_kutta4_output {"runge_kutta4.txt"};

  StateVec initial_state {initial_conditions};

  integrate(euler_integrator, sysRHS, print_coordinates, euler_output, initial_state, num_steps, dt);
  integrate(runge_kutta4_integrator, sysRHS, print_coordinates, runge_kutta4_output, initial_state, num_steps, dt);

  return 0;
}
