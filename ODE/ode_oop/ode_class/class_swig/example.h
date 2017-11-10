/* File : example.h */
#include <vector>
#include <iostream>
#include <algorithm>

typedef std::vector<double> StateVec;

class ODE {
  private:
  const int N;
  const double dt;
  double tfinal;
  int num_steps;
public:
  ODE(int iN, double itfinal, double idt, StateVec iIC) : N(iN), dt(idt) {
    tfinal = itfinal;
    num_steps = int(tfinal/dt);
    IC = iIC;
  }
  virtual ~ODE() { }

  void euler_integrator(StateVec & );
  void runge_kutta4_integrator(StateVec &);
  std::vector<StateVec> integrate (std::string);
  StateVec dydt(const StateVec &x);
  StateVec IC;
};
