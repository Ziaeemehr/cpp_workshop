#include <iostream>
#include <vector>
#include <assert.h>
#include <string>
#include <stdio.h> 
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <Eigen/Dense>

using dim1 = std::vector<double>;
using dim2 = std::vector<dim1>;

double get_wall_time();
dim2 multiplyMatrices( dim2 &A, dim2 &B);
void display_timing(double wtime, double cptime);


int main() {
    using namespace Eigen;

    int N = 1000;
    int num = 3;
    dim2 A(N, dim1(N)), AA(N, dim1(N));
    MatrixXd M(N,N), MM(N,N);

    for (int i=0; i<N; i++) {
        for (int j=0; j<N; j++) {
            double r = rand() / (RAND_MAX + 1.0);
            A[i][j] = r;
            M(i,j)  = r;
        }
    }

    double wtime1  = get_wall_time(); //timing
    for (int i=0; i<num; i++)
        MM = M*M;
    wtime1  = get_wall_time() - wtime1;
    display_timing(wtime1, 0);

    
    double wtime2  = get_wall_time(); //timing
    for (int i=0; i<num; i++)
        AA = multiplyMatrices(A, A);
    wtime2  = get_wall_time() - wtime2;
    display_timing(wtime2, 0);


    for (int i=0; i<10; i++) {
        std::cout << AA[0][i] << "\t" << MM(0,i) << std::endl;
    }

}


/*------------------------------------------------------------*/
dim2 multiplyMatrices( dim2 &A, dim2 &B)
{
    int r1 = A.size();
    int c1=A[0].size();
    int r2 = B.size();
    int c2 = B[0].size();
    
    assert(c1==r2); // "bad dimension for matrix multiplication"
    
    dim2 mult(r1, std::vector<double>(c2));

	// Multiplying matrix A and B and storing in array mult.
	for(int i = 0; i < r1; ++i) {
		for(int j = 0; j < c2; ++j) {
			for(int k=0; k<c1; ++k) {
                double tmp = A[i][k] * B[k][j];
				mult[i][j] += tmp;
			}
		}
	}
	
	return mult;
}
/*------------------------------------------------------------*/
double get_wall_time()
{
    /*measure real passed time */
    struct timeval time;
    if (gettimeofday(&time,NULL)){
        //  Handle error
        return 0;
    }
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}
//------------------------------------------------------------////------------------------------------------------------------//
void display_timing(double wtime, double cptime)
{
    int wh,ch;
    int wmin,cpmin;
    double wsec,csec;
    wh = (int) wtime/3600;
    ch = (int) cptime/3600;
    wmin = ((int)wtime % 3600)/60;
    cpmin = ((int)cptime % 3600)/60;
    wsec = wtime-(3600.*wh+60.*wmin);
    csec = cptime-(3600.*ch+60.*cpmin);
    printf ("Wall Time : %d hours and %d minutes and %.4f seconds.\n",wh,wmin,wsec);
    // printf ("CPU  Time : %d hours and %d minutes and %.4f seconds.\n",ch,cpmin,csec);
} 
/*------------------------------------------------------------*/