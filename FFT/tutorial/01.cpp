#include <iostream>
#include <cstdlib>
#include <fftw3.h>
#include <math.h>
#include <fstream>
using namespace std;

int main()
{
    const int Lx = 24;
    const int Lt = 48;
    int var_x;
    int var_T;

    double F[Lx][Lt];
    double pi = 4*atan(1);

    fftw_complex *in, *out;
    fftw_plan p;

    // Declare one-dimensional contiguous arrays of dimension   Lx*Lt
    in = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*Lx*Lt);
    out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*Lx*Lt);
    

    // create plan, an object containing necessary parameters
    p = fftw_plan_dft_2d(Lx, Lt, in, out, FFTW_FORWARD, FFTW_MEASURE);
    
    ifstream input;
    input.open("out_b5p40kp13625-24x48_pion_80-200_2d");
    // Put input data into F[x][T]
    // Careful - the loop order depends on how the data is recorded
    for(int x = 0; x < Lx; x++)
    {
	for(int T = 0; T < Lt; T++)
	{
	    input >> var_T;
	    input >> var_x;
	    input >> F[x][T];
	}
    }

    // Now turn the array F[x][T] into C-order (row major) 1-d array
    // It also happens to be an array of fftw_complex values
    // However, we only need to store each value in the real part
    // The order here is important. Rightmost index loops on the inside
    for(int x = 0; x < Lx; x++)
    {
	for(int T = 0; T < Lt; T++)
	{
	    in[T + Lt*x][0] = F[x][T];
	    // in[T + Lt*x][1] = F[x][T][1]  : Can enter imaginary part if needed
	}
    }
    input.close();

    // Perform FFT
    fftw_execute(p);

    // Get output data into 2-d form - use same array as for input
    for(int x = 0; x < Lx; x++)
    {
	for(int T = 0; T < Lt; T++)
	{
	    F[x][T] = out[T + Lt*x][0];
	    // F[x][T][1] = out[T + Lt*x][1] : Can get imaginary data too if it exists
	}
    }

    // Print output to file
    // Remember that the array indices x and y represent a different variable
    // e.g. momentum space, k = 2*pix/L
    ofstream output;
    output.open("2d_transform_out");
    for(int x = 0; x < Lx; x++)
    {
	for(int T = 0; T < Lt; T++)
	{
	    output << 2*pi*x/Lx << "\t" << 2*pi*T/Lt << "\t" << F[x][T] << "\n";
	}
	output << "\n";
    }

    fftw_destroy_plan(p);
    fftw_free(in);
    fftw_free(out);

    output.close();
}
