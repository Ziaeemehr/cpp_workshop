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

void xprsys(const int n,const dim1& x, dim1& f,
            const double ka,const dim1& w);
void solveEuler(int N,dim1 x, double ka, double dt, dim1& w,
                dim2& x_vec,dim1& times);
double get_wall_time();
double get_cpu_time();
void display_timing(double wtime, double cptime);

//------------------------------------------------------------//
//------------------------------------------------------------//

int main()
{
    int N{300};
    double ka{2.0};
    double tfinal{20};
    double dt{0.1};
    dim1 IC(N);
    int nstep = (int) tfinal/dt;

    dim2 x_vec(N,dim1(nstep));
    dim1 times(nstep);
    dim1 w(N,M_PI);

    double wtime  = get_wall_time();
    double cptime = get_cpu_time();

    for (int i=0; i<N; i++)
        // IC[i] =  (double) rand()/(RAND_MAX) * 2*M_PI;
        IC[i] = i*0.1;
    solveEuler(N,IC,ka,dt,w,x_vec,times);


    wtime = get_wall_time() - wtime;
    cptime = get_cpu_time() - cptime;
    display_timing(wtime, cptime);

    string s = "result.txt";    
    char const *filename1 = s.c_str();
    FILE *ofile; 
    ofile = fopen(filename1, "w");
    
    for (int i=0; i<nstep; i++){
        fprintf(ofile,"%15.6f",times[i]);
        for (int j=0; j<N; j++){
            fprintf(ofile,"%15.6f",x_vec[j][i]);
        }
        fprintf(ofile,"\n");
    }


    return 0;
}

//------------------------------------------------------------//
//------------------------------------------------------------//
void xprsys(const int n,const dim1& x, dim1& f,const double ka,const dim1& w)
{
    double sum1=0;
    f[0] = 0.0;
    #pragma omp parallel for reduction(+:sum1)
    for (int i=0; i<n; i++){
        sum1 = 0;
        for(int j=0; j<n; j++)
            sum1 += sin(x[j]-x[i]);
        f[i] = w[i] + ka * sum1;
    }
}

//------------------------------------------------------------//
//------------------------------------------------------------//
void solveEuler(int N,dim1 x, double ka, double dt, dim1& w,
dim2& x_vec,dim1& times)
{
    times[0] = 0;
    dim1 f(N);
    int nstep = times.size();

#pragma omp parallel for
    for (int i=0; i<N; i++){
        x_vec[i][0] = x[i];
        f[i] = 0;
    }

    for (int i=1; i<nstep; i++){
        xprsys(N,x,f,ka,w);
        times[i] = i*dt;
        #pragma omp parallel for 
        for (int j=0; j<N; j++) {
            x[j] = x[j]+ dt*f[j];
            x_vec[j][i] = x[j];
        }
    }
}
//------------------------------------------------------------//
//------------------------------------------------------------//
double get_wall_time(){
    struct timeval time;
    if (gettimeofday(&time,NULL)){
        //  Handle error
        return 0;
    }
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}
//------------------------------------------------------------//
//------------------------------------------------------------//
double get_cpu_time(){
    return (double)clock() / CLOCKS_PER_SEC;
}
//------------------------------------------------------------//
//------------------------------------------------------------//
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
    printf ("CPU  Time : %d hours and %d minutes and %.4f seconds.\n",ch,cpmin,csec);
}
//------------------------------------------------------------//
//------------------------------------------------------------//