/* File : functions.h */
#include <vector>
#include <iostream>
#include <algorithm>
#include <iostream>

class ODE;

typedef StateVec (DDE::*DeriveFunc)(const StateVec &x);
// typedef int (*printf_alias)(const char*, ...);
// printf_alias holler = std::printf;

typedef std::vector<double> StateVec;

class ODE {
  private:
  const int N;
  const double dt;
public:
  ODE(int iN, double idt, StateVec iIC) : N(iN), dt(idt) {
    IC = iIC;
  }
  virtual ~ODE() { }


  void    euler_integrator (StateVec &y );
  std::vector<StateVec> integrate (DeriveFunc dydt, int num_steps);
  StateVec oscillator(const StateVec &x);
  StateVec IC;
};
