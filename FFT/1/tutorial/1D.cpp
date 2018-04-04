#include "fftw3.h"
#include <cstdio>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>

std::vector<double> linspace(double a, double b, int n);

int main()
{
    int input_size{1024};
    //Compute corresponding number of complex output samples
    int output_size = (input_size/2 + 1);
    // Allocate input and output buffers
    double* input_buffer=static_cast<double* >(fftw_malloc(input_size*sizeof(double)));
    fftw_complex* output_buffer=static_cast<fftw_complex*>(fftw_malloc(output_size*sizeof(fftw_complex)));

    double T = 1.0/(double) input_size;

    for (int i = 0; i < input_size-1; ++i) {
        double t=i*T;
        input_buffer[i] = 1.0*sin(2.0 * M_PI * 50.0 * t );
                    // 0.5*sin(2.0 * M_PI * 80.0 * t );
    }

    int flags = FFTW_ESTIMATE;
    fftw_plan plan = fftw_plan_dft_r2c_1d(  input_size, 
                                            input_buffer, 
                                            output_buffer, 
                                            flags);
                                                
    fftw_execute(plan);
    std::vector<double> xf = linspace(0,1./(2.0*T),output_size);
    
    std::ofstream f2;
    f2.open("result.txt");

    for (int i=0; i<output_size; i++){
        double yf = (output_buffer[i][0]);
        f2 << xf[i] << " " << yf <<"\n";
    }
    f2.close();
    fftw_free(input_buffer);
    fftw_free(output_buffer);
    fftw_destroy_plan(plan);



    return 0;
}

std::vector<double> linspace(double a, double b, int n) 
{
    using namespace std;
    vector<double> arr;
    double step = (b-a) / (n-1);

    while(a <= b) {
        arr.push_back(a);
        a += step;           // could recode to better handle rounding errors
    }
    arr.push_back(a); //to include the end point
    return arr;
}