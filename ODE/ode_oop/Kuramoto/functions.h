#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <fstream>
#include <valarray>
#include <cmath>
#include <iomanip>
#include <vector>
#include <omp.h>
#include <iostream>
#include "config.h"


using StateVec   = std::valarray<double>;
using DerivFunc  = StateVec (*) (const StateVec &);
using OutputFunc = void (*) (double , const StateVec &, std::ofstream &);
using Integrator = void (*) (StateVec &, DerivFunc, double);


void euler_integrator (StateVec &, DerivFunc, double);
void runge_kutta4_integrator (StateVec &, DerivFunc, double);
void integrate (Integrator, DerivFunc, OutputFunc, 
                std::ofstream &, StateVec, int, double);
void print_coordinates (double time, const StateVec &y, 
                        std::ofstream &output);
StateVec Kuramoto (const StateVec &x);

#endif
