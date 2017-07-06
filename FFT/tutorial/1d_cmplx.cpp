/* Start reading here */

#include <fftw3.h>

#define NUM_POINTS 640

/* Never mind this bit */
#include <cstdio>
#include <cmath>
#include <iostream>
#include <fstream>

#define REAL 0
#define IMAG 1

void acquire_from_somewhere(fftw_complex* signal) {
    /* Generate two sine waves of different frequencies and
     * amplitudes.
     */

    int i;
    for (i = 0; i < NUM_POINTS; ++i) {
        double theta = (double)i / (double)NUM_POINTS * M_PI;

        signal[i][REAL] = 1.0 * cos(10.0 * theta) +
                          0.5 * cos(25.0 * theta);

        signal[i][IMAG] = 1.0 * sin(10.0 * theta) +
                          0.5 * sin(25.0 * theta);
    }
}

void do_something_with(fftw_complex* result) {

    std::ofstream myfile;
    myfile.open("magnitude.txt");
    for (int i = 0; i < NUM_POINTS; ++i) {
        double mag = sqrt(result[i][REAL] * result[i][REAL] +
                          result[i][IMAG] * result[i][IMAG]);
        myfile << mag << std::endl;
        // printf("%g\n", mag);
    }
    myfile.close();
}


/* Resume reading here */

int main() {
    fftw_complex signal[NUM_POINTS];
    fftw_complex result[NUM_POINTS];

    fftw_plan plan = fftw_plan_dft_1d(NUM_POINTS,
                                      signal,
                                      result,
                                      FFTW_FORWARD,
                                      FFTW_ESTIMATE);

    acquire_from_somewhere(signal);
    fftw_execute(plan);

    std::string s = "signal.txt";
    char const *filename = s.c_str();
    FILE *ofile; 
    ofile = fopen(filename, "w");
    for (int i=0; i<NUM_POINTS; i++){
        fprintf(ofile,"%14.9f %14.9f \n",signal[i][REAL],signal[i][IMAG]);
    }
    fclose(ofile);

    std::string s1 = "result.txt";
    char const *filename1 = s1.c_str();
    FILE *ofile1; 
    ofile1 = fopen(filename1, "w");
    for (int i=0; i<NUM_POINTS; i++){
        fprintf(ofile1,"%14.9f %14.9f \n",result[i][REAL],result[i][IMAG]);
    }
    fclose(ofile1);

    do_something_with(result);
    fftw_destroy_plan(plan);

    return 0;
}
