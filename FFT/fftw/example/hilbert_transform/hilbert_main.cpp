#include <fftw3.h>
#include <cstdio>
#include <stdio.h>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string.h>

// macros for the real and imaginary parts
#define REAL 0
#define IMAG 1
// length of the complex arrays
#define N 7
/*---------------------------------------------------------*/
void hilbert(const double *in, fftw_complex *out)
{
	// copy data into the complex array
	for (int i=0; i<N; i++) {
		out[i][REAL] = in[i];
		out[i][IMAG] = 0;
	}
	// create a DFT plan and execute it
	fftw_plan plan = fftw_plan_dft_1d(N, out, out, FFTW_FORWARD, FFTW_ESTIMATE);
	fftw_execute(plan);
	// destroy the plan to prevent a memory leak
	fftw_destroy_plan(plan);
	int hN = N>>1; 			// half of the length(N/2);
	int numRem = hN;		// the number of remaining elements

	// nultiply the appropriate values by 2
	// those that sould be multiplied by 1 are left intact because they wouldn't change
	for (int i = 1; i < hN; i++)
	{
		out[i][REAL] *= 2;
		out[i][IMAG] *= 2;
	}
 
	// if the length is even, the number of remaining elements decreases by 1
	if (N%2 ==0)
		numRem--;

	// if it is odd and greater than 1, the middle value must be multiplied by 2
	else if (N>1) {
		out[hN][REAL] *=2;
		out[hN][IMAG] *=2;
	}
	// set the remaining values to 0
	// multiply by 0 gives 0 so we don't care about multiplication
	memset(&out[hN+1][REAL], 0, numRem * sizeof(fftw_complex));

	// create an IDFT plan and execute it 
	plan = fftw_plan_dft_1d(N, out, out, FFTW_BACKWARD, FFTW_ESTIMATE); 
	fftw_execute(plan);
	// dom some cleaning
	fftw_destroy_plan(plan);
	fftw_cleanup();
	
	// scale the IDFT output
	for (int i = 0; i < N; i++)
	{
		out[i][REAL] /=N;
		out[i][IMAG] /=N;
	}


}
/*---------------------------------------------------------*/
void displayComplex(fftw_complex *y)
{
	for (int i = 0; i < N; ++i)
			std::cout << y[i][REAL] << "  " << y[i][IMAG] << " i" << std::endl;
}
		

/*---------------------------------------------------------*/
int main()
{
	// input array
	double x[N];
	// output array
	fftw_complex y[N];
	// fill the first array with some numbers
	for (int i = 0; i < N; ++i) {
		x[i] = i + 1;		// i.e., { 1, 2, 3, 4, 5, 6, 7, 8 }
	}
	// compute the FFT of x and store the results in y
	hilbert(x, y);
	// display the results
	std::cout << "Hilbert =" << std::endl;
	displayComplex(y);
	
    return 0;
}
/*---------------------------------------------------------*/
