#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

int main(int argc, char const *argv[])
{
	std::vector<double> v {1., 2., 3., 4., 5., 8.};
	double *s = new double[6];
	for (int i=0; i<v.size(); i++)
		s[i] = v[i];
	for (int i=0; i<6; i++)
		cout << s[i] << endl;

	return 0;
}