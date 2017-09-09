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

template <typename T>
using StateVec   = T<double>;

template <typename T>
using DerivFunc  = StateVec<T> (dde::*) (const StateVec<T> & );
template <typename T>
using Integrator = void (dde::* ) (StateVec<T>& , DerivFunc<T> );
using std::vector;
using std::cout;

template <typename T>
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
        void integrate (Integrator<T> , DerivFunc );
        StateVec oscillator (const StateVec &x );

};

#endif
// void dde::integrate (void (dde::*integrator) (StateVec& , DerivFunc ) , DerivFunc dydt) 