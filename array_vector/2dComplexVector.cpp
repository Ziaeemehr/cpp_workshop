#include <vector>
#include <complex>
#include "iostream"

using namespace std;

typedef std::complex<double> Complex;
typedef std::vector<Complex> ComplexVector;
typedef std::vector<ComplexVector> ComplexVector2;
int main()
{	
	ComplexVector2 V(2,ComplexVector(3));

	for (int i=0; i<V.size(); i++){
		for (int j=0; j<V[0].size(); j++){
			V[i][j] = {2,1};
		}
	}

	for (int i=0; i<V.size(); i++){
		for (int j=0; j<V[0].size(); j++){
			cout << V[i][j] << "\t";
		}
		cout << endl;
	}


	return 0;
}