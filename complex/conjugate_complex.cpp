// conj example
#include <iostream>     // std::cout
#include <complex>      // std::complex, std::conj

int main ()
{
  std::complex<double> a (10.0,2.0);
  std::complex<double> b = std::conj(a);
  std::cout << "The conjugate of " << a << " is " << b << '\n';
  std::complex<double> c(0.5,2.0);
  std::cout << (a*b) * c << "\n";

  return 0;
}