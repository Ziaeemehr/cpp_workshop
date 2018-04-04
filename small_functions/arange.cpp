#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

vector<double> linspace(double a, double b, int n) 
{
    vector<double> arr;
    double step = (b-a) / (n-1);

    while(a <= b) {
        arr.push_back(a);
        a += step;           // could recode to better handle rounding errors
    }
    arr.push_back(a); //to include the end point
    return arr;
}


vector<double> arange(const double start, const double end, const double step) 
{
    int nstep = round((end-start)/step);
    cout << "nstep = " << nstep<< endl;
    cout << "nstep = " << (end-start)/step << endl;
    vector<double> arr(nstep);

    for (int i=0; i<nstep; i++)
    	arr[i] = start + i * step;
    return arr;
}


int main(int argc, char const *argv[])
{
	auto V = arange(0,11.95,0.1);
	for (auto v:V)
		std::cout << v << std::endl;
	return 0;
}