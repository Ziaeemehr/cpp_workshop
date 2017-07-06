#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include<fstream>
#include<cstdlib>
#include<random>
#include<iostream>
#include<algorithm>
#include<functional>
#include<iostream>
#include<cstdio>
#include<cmath>
#include<string>
#include<time.h> 
#include <sys/time.h>
#include<vector>
#include <omp.h>

using namespace std;

typedef vector<double> dim1;
typedef vector<dim1>   dim2;

constexpr int N{50};
constexpr double ka{2.0};
constexpr double tfinal{100};
constexpr double dt{0.01};
extern dim1 w;


//============================================================//
void xprsys(const int n,const dim1& x, dim1& f);
void solveEuler(const int N,dim1 x, const double dt, 
    dim2& x_vec, dim1& times);
void SolveRK4(const int n, double h,dim1 x, dim2& x_vec, 
    int nstep, dim1& times);             
double get_wall_time();
double get_cpu_time();
void display_timing(double wtime, double cptime);
void SaveTofile(const std::string s, 
    const dim1& times, const dim2& x_vec);


#endif