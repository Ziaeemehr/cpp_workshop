//Simpson's 1/3rd Rule for Evaluation of Definite Integrals
#include<iostream>
#include<cmath>
#include<vector>

using namespace std;

double f(double x)
{
    return x*x*x;
}

double simp(const vector<double>& x, 
            const vector<double>& y, 
            const double a,
            const double b, 
            const int n)
{
    /*n is even and x , y are vectors with 2n+1 points
      a and b are initial and final points of integration
    */

    double s1=0, s2=0;
    double h = (b - a) / n;

    for (int i = 1; i < n; i += 2)
        s1 += y[i]; /*loop to evaluate 4*(y1+y3+y5+...+yn-1)*/
    s1 *= 4;

    for (int i = 2; i < n - 1; i += 2)
        s2 += y[i]; /*loop to evaluate 2*(y2+y4+y6+...+yn-2)*/
    s2 *= 2.0;

    return h / 3.0 * (y[0] + y[n] + s1 + s2);
}

int main()
{   

    double c, s1=0, s2=0, integral;
    double a = 0;
    double b = 1;
    int n = 100; //even number

    vector<double> x(n+1),y(n+1);
    double h = (b-a)/n;

    for (int i=0; i<n+1; i++)
    {                                //loop to evaluate x0,...xn and y0,...yn
        x[i] = a+i*h;                //and store them in arrays
        y[i] = f(x[i]);
    }

    for (int i=1; i<n; i+=2)
        s1 += y[i];                //loop to evaluate 4*(y1+y3+y5+...+yn-1)
    s1 *= 4;

    for (int i=2; i<n-1; i+=2)
        s2 += y[i];                /*loop to evaluate 4*(y1+y3+y5+...+yn-1)+
                                        2*(y2+y4+y6+...+yn-2)*/ 
    s2 *= 2.0;
    integral = h/3.0*(y[0]+y[n]+s1+s2);    //h/3*[y0+yn+4*(y1+y3+y5+...+yn-1)+2*(y2+y4+y6+...+yn-2)]
    cout << "\nIntegral = "<< integral <<"\n"<< endl;
    cout << "\nIntg = " << simp(x, y, a, b, n) << "\n"
         << endl;
    return 0;
} 