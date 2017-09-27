/* File : example.h */
#include <vector>
#include <iostream>
#include <algorithm>

typedef std::vector<double> StateVec;

class ODE {
  private:
  const int N;
  const double dt;
public:
  ODE(int iN, double idt, double r, StateVec iIC) : N(iN), dt(idt), radius(r) {
    IC = iIC;
  }
  virtual ~ODE() { }


  double  radius;
  void    print_IC();
  void    euler_integrator (StateVec &y );
  std::vector<StateVec> integrate (int num_steps);
  StateVec oscillator(const StateVec &x);
  StateVec half(const StateVec& v);
  StateVec IC;
};
