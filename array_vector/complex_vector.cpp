#include <vector>
#include <iostream>
#include <complex>

using namespace std;

typedef std::complex<double> Complex;

int main()
{
	Complex com_one;              // value 0 + 0i
	Complex com_two(3.14);        // value 3.14 + 0i
	Complex com_three(1.5, 2.0); 
	Complex com_four(com_two);    // value is also 3.14 + 0i

	std::cout << std::norm(com_three) << std::endl;
	std::cout << std::abs(com_three)  << std::endl;
	cout << sqrt(com_three.real()*com_three.real() + com_three.imag()+com_three.imag())<<"\n";
	cout << sqrt(6.25)<<"\n";

	vector<Complex> A{{1,1},{1,2}};
	cout << A[0] << "\t" << A[1] <<"\n";

	Complex com1 = {1,2.5};
	cout << com1 << endl;


	return 0;

}