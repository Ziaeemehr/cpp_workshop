#include <vector>
#include <iostream>
#include <complex>

using namespace std;
typedef std::complex<double> Complex;
typedef std::vector<std::vector<long unsigned int> > dim2I;


class Myclass {
public:
	static const Complex ii;
	static const int B;
};

const Complex Myclass::ii(1,2);
const int Myclass::B=9;
int main()
{
	dim2I a{{10}};
	cout << a[0][0]<<endl;
	Myclass A;

	cout << A.ii<< endl;
	cout << A.B << endl;

	Complex b1(2,4);
	Complex b2 = {2,4};

	cout << b1 << "\t" << b2 << endl;
	return 0;
}