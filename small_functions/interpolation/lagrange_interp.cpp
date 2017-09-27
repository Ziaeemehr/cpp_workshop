#include <vector>
#include <iostream>
#include <cmath>
#include <assert.h>

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))


using namespace std; 

std::vector<double> linspace(double , double , int );
double lagrange_interp(const std::vector<double> &,
                       const std::vector<double> &,
                       double , int & , int );


int main()
{
    int N  {63};
    int n  {126};
    int n0 {0};
    vector<double> X(N),Y(N);
    vector<double> x(n), y(n), y_exact(n);

    X = linspace(0,2*M_PI,N);
    x = linspace(0,2*M_PI,n);
    // cout << X[X.size()-1] <<" "<<x[x.size()-1]<<"\n";
    
    for (int i=0; i<N; i++)
        Y[i] = sin(X[i]);
    for (int i=0; i<n; i++)
        y_exact[i] = sin(x[i]);

    for (int i=0; i<n; i++)
        y[i] = lagrange_interp(X, Y, x[i], n0, 2);

    for (int i=0; i<n; i++)
        std::cout << x[i] <<" "<< y[i] << " " << y_exact[i]<<"\n";
    double max_error = 0.0;
    for (int i=0; i<n; i++)
    {
        double err = fabs(y_exact[i]-y[i]);
        if ( err > max_error)
            max_error = err;
    }
    std::cout << max_error <<"\n";
    return 0;
}

/*------------------------------------------------------------*/
double lagrange_interp(const std::vector<double> &X,
                       const std::vector<double> &Y,
                       double x, int & n0, int order)
{
    int N = X.size();
    if (x>X[N-1] || x<X[0])
    {
        std::cerr << "data out of interval [X_min, X_max]" <<"\n";
        std::cout << x <<  "["<<X[0] << "," <<X[N-1]<<"]" <<"\n";
        exit(2);
    }
    /* find left end of interval for interpolation */
    if (x > X[N-2])
    {
        n0 = N-2;
    }
    else 
    {
        while (x > X[n0+1])
            n0++;
    }

    // linear interpolation
    if (order == 1)
    {
        std::vector<double> xp(order+1);
        std::vector<double> yp(order+1);

        for (int i=0; i<order+1; i++)
        {
           xp[i] = X[n0+i];
           yp[i] = Y[n0+i];
        }
        double dydx = (yp[1]-yp[0])/(xp[1]-xp[0]);

        return yp[0]+dydx*(x-xp[0]);
    }

    // quadratic interpolation 
    if (order == 2)
    {
        std::vector<double> xp(order+1);
        std::vector<double> yp(order+1);
        std::vector<double> L(order+1);

        if(n0 < N-2)
        {
            for (int i=0; i<order+1; i++)
            {
                xp[i] = X[n0+i];
                yp[i] = Y[n0+i];
            }
        }
        else
        {
            for (int i=0; i<order+1; i++)
            {
                xp[i] = X[n0-1+i];
                yp[i] = Y[n0-1+i];
            }
        }
        for (int k=0; k<order+1; k++)
        {
            double term_N = 1.0;
            double term_D = 1.0;
            for (int j=0; j<order+1; j++)
            {
                if (j != k)
                {
                    term_N *= (x - xp[j]);
                    term_D *= (xp[k]- xp[j]);
                }
            }
            L[k] = term_N/term_D;
        }
        double Sum = 0.0;
        for (int i=0; i<order+1; i++)
            Sum += L[i]*yp[i];

        return Sum;
    }
}

/*------------------------------------------------------------*/
std::vector<double> linspace(double a, double b, int n) 
{
    std::vector<double> arr;
    double step = (b-a) / (n);

    while(a <= b) {
        arr.push_back(a);
        a += step; 
    }
    arr.push_back(a);
    return arr;
}