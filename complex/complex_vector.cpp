#include <complex>
#include <vector>
#include <iostream>

// using namespace std;
//using namespace std::complex_literals;

int main()
{
    std::vector<std::complex<double>> x(10);
    for (int i=0; i<x.size(); i++)
        x[i] = {i+0.0,i*2.0};
    
    for (auto i : x)
        std::cout << i << std::endl;
    
    std::complex<double> Uf[2]={{1, 2},{3, 4}};
    std::complex<double> a(1.0,2.0);
    std::complex<double> b(3.0,4.0);
    std::cout << a*b<< std::endl;

    
    return 0;
}