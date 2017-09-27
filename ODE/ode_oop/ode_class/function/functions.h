#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <fstream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <iostream>


typedef  std::vector<double> StateVec;
typedef  std::vector<std::vector<double> > StateVec2;

StateVec sysRHS (StateVec &y);
void euler_integrator (StateVec &, double);
StateVec2 integrate ( StateVec, int, double); 

#endif
