#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "interpolation.h"

using namespace alglib;


int main(int argc, char **argv)
{
    //
    // We use piecewise linear spline to interpolate f(x)=x^2 sampled 
    // at 5 equidistant nodes on [-1,+1].
    //
    real_1d_array x = "[-1.0,-0.5,0.0,+0.5,+1.0]";
    real_1d_array y = "[+1.0,0.25,0.0,0.25,+1.0]";
    double t = 0.25;
    double v;
    spline1dinterpolant s;

    // build spline
    spline1dbuildlinear(x, y, s);

    // calculate S(0.25) - it is quite different from 0.25^2=0.0625
    v = spline1dcalc(s, t);
    printf("%.4f\n", double(v)); // EXPECTED: 0.125
    return 0;
}