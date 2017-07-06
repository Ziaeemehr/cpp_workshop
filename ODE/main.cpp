#include "functions.h"

/*comparing RK4 and Euler integration performance and accuracy
 * notice that dt should be changed to compare the performance
 * with the same accuracy Rk4 is >30 times faster than Euler 
*/

dim1 w;

int main()
{
    
    dim1 IC(N);
    int nstep = (int) tfinal/dt;

    dim2 x_vec(nstep,dim1(N));
    dim1 times(nstep);
    dim1 w0(N,M_PI);
    w = w0;
    double wtime,cptime;

    for (int i=0; i<N; i++)
        IC[i] = i*0.1;
        // IC[i] =  (double) rand()/(RAND_MAX) * 2*M_PI;

    // wtime  = get_wall_time();
    // cptime = get_cpu_time();        
    // solveEuler(N,IC,dt,x_vec,times);

    // wtime = get_wall_time() - wtime;
    // cptime = get_cpu_time() - cptime;
    // cout << "Euler"<<'\n';
    // display_timing(wtime, cptime);

    // SaveTofile("euler.txt", times, x_vec);
    

    wtime  = get_wall_time();
    cptime = get_cpu_time();

    SolveRK4(N,dt,IC,x_vec,nstep,times);

    wtime = get_wall_time() - wtime;
    cptime = get_cpu_time() - cptime;
    cout << "RK4"<<'\n';
    display_timing(wtime, cptime);

    SaveTofile("rk4.txt", times, x_vec);

    return 0;
}
