#include "random_seedo.h"
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char const *argv[])
{
	INITIALIZE_RANDOM_F(234);
	// for (int i=0; i<10; i++)
	// {
	// 	double r = RANDOM *10.0;
	// 	cout << r << "\n";
	// }
	// for (int i=0; i<20; i++)
	// {
	// 	int rnd = RANDOM_INT(2); 
	// 	cout <<  rnd * 2 - 1 << endl;
	// }

	for (int i=0; i<20; i++)
	{
		double rnd = RANDOM_GAUSS(1.0) * 2.0 - 10.0; 
		cout <<  rnd << endl;
	}


	FREE_RANDOM;



	return 0;
}

