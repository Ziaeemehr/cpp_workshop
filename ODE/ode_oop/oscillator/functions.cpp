#include "functions.h"

void euler_integrator (StateVec &y, DerivFunc dydt, double dt) {
  y += dydt (y) * dt;
}


void runge_kutta4_integrator (StateVec &y, DerivFunc dydt, double dt) {
  auto k1 = dydt (y);
  auto k2 = dydt (y + dt*k1/2.);
  auto k3 = dydt (y + dt*k2/2.);
  auto k4 = dydt (y + dt*k3);
  y += (k1 + 2.*k2 + 2.*k3 + k4) * dt/6;
}

void integrate (Integrator integrator, DerivFunc dydt, OutputFunc output_func, 
                std::ofstream &output, StateVec y, int num_steps, double dt) {
  for (auto step = 0; step < num_steps; ++step) {
    output_func (step*dt, y, output);
    integrator (y, dydt, dt);
  }
}
