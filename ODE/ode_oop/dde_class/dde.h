#ifndef DDE_H
#define DDE_H

#include <cstdlib>
#include <random>
#include <iostream>
#include <algorithm>
#include <functional>
#include <iostream>
#include <cstdio>
#include <cmath>
#include <string>
#include <vector>
#include <valarray>

// using namespace std;

class dde;

using StateVec   = std::valarray<double>;
using DerivFunc  = StateVec (dde::*) (const StateVec & );
using Integrator = void (dde::* ) (StateVec& , DerivFunc );
using std::vector;
using std::cout;

class dde 
{
    private:
        const int N;
        const double dt;
        StateVec IC;

    public:
        dde(int iN, double idt, StateVec iIC) :N(iN),dt(idt)
        {
            IC = iIC;
            // dt = idt;
        }
        void euler_integrator (StateVec&, DerivFunc );
        void integrate (Integrator , DerivFunc );
        StateVec oscillator (const StateVec &x );

};

#endif
