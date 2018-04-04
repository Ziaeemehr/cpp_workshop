#include "functions.h"

#define NUM_POINTS 1001


int main()
{
    int n{NUM_POINTS};
    int m{NUM_POINTS};
    fftw_complex a_sig[NUM_POINTS];
    fftw_complex b_sig[NUM_POINTS];
    fftw_complex result[NUM_POINTS];
    
    //create 2 sample signals
    dim1 t_steps = linspace(0,1,n);
    for (int i=0; i<n; i++) {
        a_sig[i][0] = sin(2*M_PI*t_steps[i]*4+5);
        b_sig[i][0] = sin(2*M_PI*t_steps[i]*4);
    }

    xcorr(a_sig, b_sig, result, NUM_POINTS);

    // print output
    for (int i=0; i<NUM_POINTS; i++){
        std::cout << result[i][0]<<std::endl;
    }

    return 0;
}