#include <vector>
#include <iostream>
#include <math.h>

using namespace std;

double f(double x)
{
    double sqx = sqrt(x);
    return 2 + sin( 2 * sqx);
    // return 2*x-sqx*cos(2*sqx)+0.5*sin(sqx);
}
/*---------------------------------------------------------------------------*/
double trap(double a, double b, int n)
{
    double h = (b - a) / (n - 1);
    double value = 0.5 * (f(a) + f(b));
    for (int k = 1; k < n-1; k++)
    {
        value += f(a + h * k);
    }
    value *= h;
    return value;
}
/*---------------------------------------------------------------------------*/
double trapf(vector<double>& t, vector<double>& v)
{
    double h = t[1]-t[0];
    int n = t.size();
    double value = 0.5 * (v[0] + v[n-1]);
    for (int k = 1; k < n-1; k++)
        value += v[k];
    value *= h;

    return value;
}
/*---------------------------------------------------------------------------*/

int main(int argc, char *argv[])
{
    int a = 1;
    int b = 6;
    for (int M = 10; M<170; M+=10)
    {
        double h = (b-a)/(double) M;
        cout << " \t M = " << M 
             << " \t h = " << h 
             << "\t integrate = " <<  trap(a, b, M) 
             << endl;
    }
    
    cout << "==================================================================="<<endl;
    for (int M=10; M<170; M+=10)
    {
        vector<double> v(M+1);
        vector<double> t(M+1);
        double h = (b-a)/(double) M;
        for (int k=0; k<(M+1); k++)
        {
            double x = a + h * k;
            v[k] = f(x);
            t[k] = x;
        }
        cout << " \t M = " << M
             << " \t h = " << h
             << "\t integrate = " << trapf(t, v)
             << endl;
    }

    return 0;
}