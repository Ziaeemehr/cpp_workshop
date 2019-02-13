// arg complex example
#include <iostream> // std::cout
#include <complex>  // std::complex, std::abs, std::arg

int main()
{
    std::complex<double> mycomplex(3.0, 4.0);

    std::cout << "The polar form of " << mycomplex;
    std::cout << " is " << std::abs(mycomplex) << "*e^i*" << std::arg(mycomplex) << "rad\n";

    return 0;
}