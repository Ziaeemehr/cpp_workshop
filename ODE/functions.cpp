#include "functions.h"

//------------------------------------------------------------//
//------------------------------------------------------------//
void xprsys(const int n,const dim1& x, dim1& f)
{
    double sum1=0;
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
void solveEuler(const int N,dim1 x, const double dt, 
                dim2& x_vec, dim1& times)
{
    times[0] = 0;
    dim1 f(N);
    int nstep = times.size();
// x_vec[nstepxN]

#pragma omp parallel for
    for (int i=0; i<N; i++)
        x_vec[0][i] = x[i];

    for (int i=1; i<nstep; i++){
        xprsys(N,x,f);
        times[i] = i*dt;
        #pragma omp parallel for 
        for (int j=0; j<N; j++) {
            x[j] = x[j]+ dt*f[j];
            x_vec[i][j] = x[j];
        }
    }
}
//------------------------------------------------------------//
//------------------------------------------------------------//
void SolveRK4(const int n, double h,dim1 x, dim2& x_vec, 
          int nstep, dim1& times)
{
    times[0] = 0.0;
    dim1 y(n);
    dim1 f1(n),f2(n),f3(n),f4(n);
    double half_h = 0.5 * h;
    double h_sixth = h/6.0;
    // x_vec[nstepxN]

    for (int i=0; i<n; i++)
        x_vec[0][i] = x[i];

    for (int k=1; k<nstep; k++){
        times[k] = k*h;
    	xprsys(n,x,f1);
        
        #pragma omp parallel for 
	    for(int i=0; i<n; i++)
	        y[i] = x[i] + half_h * f1[i];
	    
        #pragma omp master
	    xprsys(n,y,f2);
        #pragma omp barrier
        
        #pragma omp for
	    for(int i=0; i<n; i++)
	        y[i] = x[i] + half_h * f2[i];
	    
        #pragma omp master
	    xprsys(n,y,f3);
        #pragma omp barrier
        
        #pragma omp for
	    for(int i=0; i<n; i++)
	        y[i] = x[i] + h * f3[i];
	    
        #pragma omp master
	    xprsys(n,y,f4);
        #pragma omp barrier

        #pragma omp for 
	    for(int j=0; j<n; j++) {
	        x[j] = x[j] + h_sixth * (f1[j] + f4[j] + 2.0 * (f2[j] + f3[j]));
            x_vec[k][j] = x[j];
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
    // printf ("CPU  Time : %d hours and %d minutes and %.4f seconds.\n",ch,cpmin,csec);
}
//------------------------------------------------------------//
//------------------------------------------------------------//
void SaveTofile(const std::string s, const dim1& times, const dim2& x_vec )
{    
    char const *filename = s.c_str();
    FILE *ofile; 
    ofile = fopen(filename, "w");
    int nstep = x_vec.size();

    for (int i=0; i<nstep; i++){
        fprintf(ofile, "%15.6f", times[i]);
        for (int j=0; j<N; j++){
            fprintf(ofile,"%15.6f",x_vec[i][j]);
        }
        fprintf(ofile,"\n");
    }
    fclose(ofile);
}
