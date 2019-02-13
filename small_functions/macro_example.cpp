#include <iostream>

#define PLUSMINUC(a) ((a > 0.0)? 1.0 : -1.0)


using namespace std;



int main(int argc, char const *argv[])
{

	for (int i=0; i<10; i++)
	{
		double a = (double)rand() / ((double)(RAND_MAX)) * 2.0 - 1.0 ;
		cout << a << "\t" << PLUSMINUC(a) << endl;	
	}
	return 0;
}