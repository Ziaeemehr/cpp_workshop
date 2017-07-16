#include <vector>
#include <valarray>
#include <iostream>
#include <ctime>
#include <numeric>
#include <sys/time.h>

// with -O2 optimization flag they are the same.

using namespace std;

void display_timing(double wtime, double cptime);
double get_wall_time();
double get_cpu_time();

int main()
{
    const int N = 10000000;    // adjust as needed
    
    clock_t start,finish;

    {
        valarray<double> a(1.0,N);
        valarray<double> b(2.0,N);
        valarray<double> c(N);

        double wtime  = get_wall_time();
        
                
        for (int i=0; i<100; i++)
            c = 2.0*a + 5.5*b; // using a for loop is similar performance to vector
        

        double sum = c.sum();

        wtime = get_wall_time() - wtime;
        display_timing(wtime, 0);
        cout << "check : sum = " << sum << endl;
    }

    {
        vector<double> a(N,1.0);
        vector<double> b(N,2.0);
        vector<double> c(N);

        double wtime  = get_wall_time();

        for (int k=0; k<100; k++) {
            for (int i=0; i<N; ++i)
                c[i] = 2.0*a[i] + 5.5*b[i];
        }
        
        double sum = accumulate(c.begin(),c.end(),0.0);

        wtime = get_wall_time() - wtime;
        display_timing(wtime, 0);
        cout << "check : sum = " << sum << endl;
    }

    return 0;
}


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
}
