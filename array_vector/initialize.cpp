#include <vector>
#include <iostream>

using namespace std;

int main(int argc, char const *argv[])
{
	const int arr[] = {16,2,77,29};
	vector<int> vec (arr, arr + sizeof(arr) / sizeof(arr[0]) );

	for (int i=0; i<vec.size(); i++)
		cout << vec[i] << endl;


	double *NU = new double[5];
	// NU = {1.,5.,7.,1.,9.};
	for (int i=0; i<5; i++)
	{
		NU[i] = i*.2;
	}
	
	return 0;
}