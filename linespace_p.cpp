#include <iostream>
#include <cstdlib>
#include <vector>
#include <array>
using namespace std;

vector<double> linspace(double a, double b, int n) 
{
    vector<double> arr;
    double step = (b-a) / (n-1);

    while(a <= b) {
        arr.push_back(a);
        a += step;           // could recode to better handle rounding errors
    }
    return arr;
}

int main()
{
    double a = 0.;
    double b = 10.;
    int n = 101;
    vector<double> t(101);
    
    t = linspace(a,b,n);
    for (int i=0; i<n; i++){
        cout << t[i] << endl;
    }
    return 0;
}