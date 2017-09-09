#include "dde.h"
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

using namespace std;

int main()
{
    int N{2};
    double dt{0.01};
    StateVec<> IC = {0.5,1};

    dde<> DDE(N, dt, IC);
    DDE.integrate(&dde::euler_integrator,&dde::oscillator);

    return 0;
}

