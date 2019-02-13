// complex::real example
#include <iostream>     // std::cout
#include <complex>      // std::complex


int main ()
{
  std::complex<double> mycomplex (10.0,1.0);
  std::complex<double> I (0., 1.0);

  std::cout << "Real part: " << mycomplex.real() << '\n';
  std::cout << "imag part: " << mycomplex.imag() << '\n';
  std::cout << 3.0*I << '\n';

  return 0;
}
