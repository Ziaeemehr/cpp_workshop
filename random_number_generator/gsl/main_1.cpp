#include "random_seedo.h"
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char const *argv[])
{
	{
		for (int i=0; i<5; i++)
		{
			INITIALIZE_RANDOM_F(234);
			cout << RANDOM << endl;
		}
			FREE_RANDOM;
	}
	{
		INITIALIZE_RANDOM_F(234);
		for (int i = 0; i < 5; i++)
		{
			cout << RANDOM << endl;
		}
		FREE_RANDOM;
	}

	return 0;
}
