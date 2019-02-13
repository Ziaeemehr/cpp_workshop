// abs complex example
#include <iostream>     // std::cout
#include <complex>      // std::complex, std::abs

int main ()
{
  std::complex<double> mycomplex (3.0,4.0);

  std::cout << "The absolute value of " << mycomplex << " is " << std::abs(mycomplex) << '\n';

  return 0;
}