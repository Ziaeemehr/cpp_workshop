#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include <fftw3.h>
#include <complex.h>
#include <fstream>
#include <assert.h>
#include <cstdlib>
#include <random>
#include <iostream>
#include <cstdio>
#include <cmath>
#include <string>
#include <time.h> 
#include <sys/time.h>
#include <vector>
#include <array>


using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::to_string;

typedef vector<double> dim1;
typedef vector<dim1> dim2;

void xcorr(fftw_complex *, fftw_complex *, fftw_complex *, int);

vector<double> linspace(double a, double b, int n) 
{
    vector<double> arr;
    double step = (b-a) / (n-1);

    while(a <= b) {
        arr.push_back(a);
        a += step;           // could recode to better handle rounding errors
    }
    arr.push_back(a); //to include the end point
    return arr;
}

#endif